#include "./DisplayKernel/OgreUtils/CameraManager.h"
#include "./DisplayKernel/OgreUtils/CameraData.h"
#include "./DisplayKernel/OgreUtils/ViewportData.h"
#include "./DisplayKernel/OgreUtils/AttachPointData.h"
//#include "./CommunicationElements/CommunicationLayer.h"
#include "./FileReaders/GenericConfigReader/GenericConfigReader.h"
#include "./FileReaders/BasicTypesReader.h"	

using namespace OgreUtils;

	CameraManager::CameraManager(){;}

	boost::shared_ptr<CameraManager> CameraManager::instance(){
		static boost::shared_ptr<CameraManager> uniqueInstance(new CameraManager());
		return uniqueInstance;
	}

	void CameraManager::init(){
		reset();
	}

	bool CameraManager::addCamera(CameraData* cam){
		std::list<CameraData*>::iterator it;
		for(it=allCameras.begin();it!=allCameras.end();it++)
			if((*it)->getCameraName()==cam->getCameraName())
				return false;
		//We can add it!
		allCameras.push_back(cam);
		pendingCameras.push_back(cam);
		return true;
	}

	bool CameraManager::addViewport(ViewportData* vp){
		std::list<ViewportData*>::iterator it;
		for(it=allViewports.begin();it!=allViewports.end();it++)
			if((*it)->getViewportName()==vp->getViewportName())
				return false;
		//We can add it!
		allViewports.push_back(vp);
		cameraViewports[vp->getCameraName()].push_back(vp);
		return true;
	}

	/**
	Check if a newly inserted node is the AttachPoint of any our cameras. 
	In that case the camera is created and any associated viewports are initialyzed. 
	*/
	bool CameraManager::checkNode(Ogre::SceneNode* node,Ogre::SceneManager* mgr){
		bool result=false;
		std::string nodeName=node->getName();
		//Look for any cameras associated to this node.
		std::list<CameraData*>::iterator it;
		for(it=pendingCameras.begin();it!=pendingCameras.end();)
			if((*it)->getCameraNodeName()==nodeName){
				(*it)->declareCamera(mgr);
				bindCameraToViewports(*it);
				(*it)->getCameraNode().getParentSceneNode()->removeChild(&(*it)->getCameraNode());
				node->addChild(&(*it)->getCameraNode());
				//Remove from the pending list.
				pendingCameras.erase(it);//It is not pending...
				it=pendingCameras.begin();
				result=true;
			}
			else it++;
			return result;
	}

	bool CameraManager::isMainCamera(Ogre::SceneNode* node,Ogre::SceneManager* mgr){
		return (*(this->allCameras.begin()))->getCameraNodeName()==node->getName();
	}

	/**
	Makes all the cameras to update their position/orientation/projection volumes. 
	If it is called every frame, it is an easy way to assure all the cameras are up-to-date. 
	Can be too consuming if there are many viewports.
	*/
	void CameraManager::updateAllCameras(){
		std::list<ViewportData*>::iterator it;
		for(it=allViewports.begin();it!=allViewports.end();it++)
			if((*it)->isBound())
				getCamera((*it)->getCameraName())->update();
	}


	/**
	Returns the camera with name cameraName
	*/
	CameraData* CameraManager::getCamera(std::string cameraName){
		std::list<CameraData*>::iterator it;
		for(it=allCameras.begin();it!=allCameras.end();it++)
			if((*it)->getCameraName()==cameraName)
				return (*it);
		return NULL;
	}

	/**
	Returns the ViewportData with name viewportName.
	*/
	ViewportData* CameraManager::getViewport(std::string viewportName){
		std::list<ViewportData*>::iterator it;
		for(it=allViewports.begin();it!=allViewports.end();it++)
			if((*it)->getViewportName()==viewportName)
				return (*it);
		return NULL;
	}

	/**
	Parses all the subfields looking for Camera and Viewport settings.
	*/
	void CameraManager::readConfig(FieldData* configuration){
		int numViewport=0;
		FieldData* subfield;
		configuration->resetSubfields();
		while(configuration->hasNextSubfield()){
			subfield=configuration->getNextSubfield();
			if(subfield->getFieldName()=="HMDCamera")
				readHMDCamera(subfield);
			else if(subfield->getFieldName()=="ProjectionCamera")
				readProjectionCamera(subfield);
			else if(subfield->getFieldName()=="DynamicCamera")
				readDynamicCamera(subfield);
			else if(subfield->getFieldName()=="Viewport"){
				readViewport(subfield,numViewport);
				numViewport++;
			}

		}
	}

	void CameraManager::reset(){
		//Reset cameras
		pendingCameras.erase(pendingCameras.begin(),pendingCameras.end());
		std::list<CameraData*>::iterator itCam;
		for(itCam=allCameras.begin();itCam!=allCameras.end();itCam++){
			(*itCam)->reset();
			pendingCameras.push_back(*itCam);
		}
		
		//Reset viewports
		cameraViewports.erase(cameraViewports.begin(),cameraViewports.end());
		std::list<ViewportData*>::iterator itVp;
		for(itVp=allViewports.begin();itVp!=allViewports.end();itVp++){
			(*itVp)->reset();
			cameraViewports[(*itVp)->getCameraName()].push_back((*itVp));
		}
	}

	/**
	This method is called when a camera is being initialized. It sets up all the viewports associated to the camera.
	*/
	void CameraManager::bindCameraToViewports(CameraData* cam){
		if(cameraViewports.find(cam->getCameraName())!=cameraViewports.end()){
			std::list<ViewportData*> l=cameraViewports[cam->getCameraName()];
			std::list<ViewportData*>::iterator it;
			for(it=l.begin();it!=l.end();it++)
				(*it)->bind(&cam->getCamera());
			cameraViewports.erase(cameraViewports.find(cam->getCameraName()));
		}
	}

	void CameraManager::readHMDCamera(FieldData* f){
		//Data to read (and default values)
		std::string cameraName;
		if(f->getFieldValue()=="")
			cameraName="Default";
		else 
			cameraName=f->getFieldValue();
		std::string nodeName="";
		float aspectRatio=4/3;
		Ogre::Matrix4 fromCameraToNode=Ogre::Matrix4::IDENTITY;
		float x_offset=0;
		float y_offset=0;
		float FOVy=0.73f;

		//Read subfields
		f->resetSubfields();
		while(f->hasNextSubfield()){
			FieldData * subfield=f->getNextSubfield();
			std::string fieldName=subfield->getFieldName();
			if(fieldName=="aspectRatio")
				aspectRatio=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="x_offset")
				x_offset=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="y_offset")
				y_offset=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="FOVy")
				FOVy=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="AttachPoint"){
				AttachPointData a=readAttachPoint(subfield);
				nodeName=a.getNodeName();
				fromCameraToNode=a.position;
			}			
		}
		//Create camera and add it to the list:
		HMDCameraData* cam=new HMDCameraData(cameraName,nodeName,aspectRatio,fromCameraToNode,x_offset,y_offset,FOVy);
		this->addCamera(cam);
	}
	void CameraManager::readProjectionCamera(FieldData* f){
		//Data to read (and default values)
		std::string cameraName;
		if(f->getFieldValue()=="")
			cameraName="Default";
		else 
			cameraName=f->getFieldValue();
		std::string nodeName="";
		float width, height;
		Ogre::Matrix4 fromCameraToNode=Ogre::Matrix4::IDENTITY;
		Ogre::Matrix4 screenPos=Ogre::Matrix4::IDENTITY;
		

		//Read subfields
		f->resetSubfields();
		while(f->hasNextSubfield()){
			FieldData * subfield=f->getNextSubfield();
			std::string fieldName=subfield->getFieldName();
			if(fieldName=="width")
				width=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="height")
				height=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="screenPosition")
				screenPos=Operation::fromStrToMatrix(subfield->getFieldValue());
			else if(fieldName=="AttachPoint"){
				AttachPointData a=readAttachPoint(subfield);
				nodeName=a.getNodeName();
				fromCameraToNode=a.position;
			}			
		}
		//Create camera and add it to the list:
		ProjectionCameraData* cam=new ProjectionCameraData(cameraName,nodeName,screenPos,width,height,fromCameraToNode);
		this->addCamera(cam);
	}
	void CameraManager::readDynamicCamera(FieldData* f){
		//Read subfields
		/*f->resetSubfields();
		while(f->hasNextSubfield()){
			FieldData * subfield=f->getNextSubfield();
			std::string fieldName=subfield->getFieldName();
			if(fieldName=="leftHandOID")
				;//this->leftHandOID=Dictionary::fromStrToOID(subfield->getFieldValue());
			
		}*/
	}
	
	void CameraManager::readViewport(FieldData* f,int z_order){
		//Data to read (and default values)
		float width=1;
		float height=1;
		float x_offset=0;
		float y_offset=0;
		ID cameraName="Default";
		ID viewportName;
		if(f->getFieldValue()=="")
			viewportName="Default";
		else 
			viewportName=f->getFieldValue();
		//Read subfields
		f->resetSubfields();
		while(f->hasNextSubfield()){
			FieldData * subfield=f->getNextSubfield();
			std::string fieldName=subfield->getFieldName();
			if(fieldName=="width")
				width=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="height")
				height=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="x_offset")
				x_offset=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="y_offset")
				y_offset=Operation::fromStrToFloat(subfield->getFieldValue());
			else if(fieldName=="camera")
				cameraName=subfield->getFieldValue();			
		}
		//Build Viewport
		ViewportData* v=new ViewportData(viewportName,x_offset,y_offset,width,height,cameraName,z_order);
		this->addViewport(v);
	}

	AttachPointData CameraManager::readAttachPoint(FieldData* f){
		//Data to read (and default values)
		ID representation="";
		Ogre::Matrix4 position=Ogre::Matrix4::IDENTITY;
		
		//Read subfields
		f->resetSubfields();
		while(f->hasNextSubfield()){
			FieldData * subfield=f->getNextSubfield();
			std::string fieldName=subfield->getFieldName();
			if(fieldName=="attachPoint")
				representation=subfield->getFieldValue();
			else if(fieldName=="position")
				position=Operation::fromStrToMatrix(subfield->getFieldValue());			
		}
		//Create AttachPoint
		AttachPointData a(representation);
		a.position=position;
		return a;
	}
