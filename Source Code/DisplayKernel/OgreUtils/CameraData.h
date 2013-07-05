#ifndef _CAMERA_DATA
#define _CAMERA_DATA
#include <string>
#include <Ogre.h>

namespace OgreUtils{
//We have to detect whenever the camera position to the screen changes. What other nodes should we check?
//Describes any camera looking towards its Z axis (0,0,1).
//Once configured, it is only necessary to call update once everyframe.
class CameraData{
protected:
	float aspectRatio, x_offset, y_offset,FOVy;
	Ogre::Matrix4 fromCameraToNode;
	std::string cameraName;
	std::string nodeName;
	Ogre::Camera* camera;
	Ogre::SceneNode* upperNode;
public:
	CameraData(std::string cameraName, std::string nodeName,Ogre::Matrix4 fromCameraToNode, float aspectRatio)
	: cameraName(cameraName)
	, nodeName(nodeName)
	, fromCameraToNode(fromCameraToNode)
	, aspectRatio(aspectRatio)
	, x_offset(0), y_offset(0), FOVy(1), camera(NULL), upperNode(NULL)
	{;}

	virtual void reset(){

		camera=NULL;
		upperNode=NULL;		
	}

	virtual Ogre::SceneNode& declareCamera(Ogre::SceneManager* mgr){
		//Create camera
		camera=mgr->createCamera(cameraName);
		camera->setAspectRatio(aspectRatio);
		camera->lookAt(Ogre::Vector3(0,0,1));
	    	camera->setNearClipDistance(0.5);
		camera->setVisible(false);
		//Create upper node and attach it to the node.
		upperNode=mgr->getRootSceneNode()->createChildSceneNode( cameraName+"CameraNode" );
		upperNode->attachObject(camera);
		//Set the position/orientation/projection volume of the camera.
		reorientCamera();
		recalculateFrustum();
		return *upperNode;
	}

	inline Ogre::SceneNode& getCameraNode(){
		return *upperNode;	
	}

	inline const std::string& getCameraNodeName(){
		return nodeName;	
	}

	inline Ogre::Camera& getCamera(){
		return *camera;	
	}

	inline const std::string& getCameraName(){
		return cameraName;	
	}

	virtual void setScreenPos(Ogre::Matrix4 screenVW){
		;
	}	

	//This method returns true if the volume of projection needs to be recalculated because of a movement in the position of nodeName
	virtual bool needRecalculate(std::string nodeName)=0;//Not used

	//This method calculates the new projection data and sets it.
	virtual void update()=0;

protected:

	//This method sets the new position for the camera. Depends on the overal positions of the camera and the screen.
	virtual void reorientCamera()=0;

	//This method recalculates the projection datta of the camera (x_offset,y_offset and FOVy) and sets them.
	virtual void recalculateFrustum()=0;

	void calculateProjectionData(Ogre::Matrix4 headPos,Ogre::Matrix4 screenPos,float width,float height,float *FOVy,float *dx,float *dy){
		Ogre::Vector4 hPos=screenPos.inverse() * headPos*Ogre::Vector4(0,0,0,1);
		//Ogre::Vector4 headOrient=screenRealPos.inverse() *()
		double tgAngle= height/(2*hPos[2]);
		*FOVy=static_cast<float>( 2* atan(tgAngle));
		*dx=hPos[0]/width;
		*dy=hPos[1]/height;
		
	}
};

class HMDCameraData: public CameraData{
public:	
	HMDCameraData(std::string cameraName, std::string nodeName,float aspectRatio,Ogre::Matrix4 fromCameraToNode, float x_offset, float y_offset, float FOVy):CameraData(cameraName,nodeName,fromCameraToNode,aspectRatio){
		this->x_offset=x_offset;
		this->y_offset=y_offset;
		this->FOVy=FOVy;
	}
	virtual bool needRecalculate(std::string nodeName){
		return false;
	}
protected:
	virtual void reorientCamera(){
		Ogre::Vector4 position=fromCameraToNode*Ogre::Vector4(0,0,0,1);
		upperNode->setPosition(position[0],position[1],position[2]);
		upperNode->setOrientation(fromCameraToNode.extractQuaternion());
		upperNode->setScale(Ogre::Vector3(1,1,1));
	}

	virtual void recalculateFrustum(){
		camera->setAspectRatio(aspectRatio);
		//setFov
		camera->setFOVy(Ogre::Radian(FOVy));
		//setOffset		
		camera->setFrustumOffset(-x_offset,-y_offset);

	}

	virtual void update(){
		camera->setVisible(false);
	}

};

class ProjectionCameraData: public CameraData{
	Ogre::Matrix4 screenPos;
	Ogre::Matrix4 cameraPosition;//Position of the observer (looking in the Z direction), does not need to be equal to the orientation of the camera (camera is perpendicular to its screen). 
	float width;
	float height;
public:	
	ProjectionCameraData(std::string cameraName, std::string nodeName,Ogre::Matrix4 screenPos,float width,float height, Ogre::Matrix4 fromCameraToNode)
	: CameraData(cameraName,nodeName,fromCameraToNode,width/height)
	, width(width)
	, height(height)
	, screenPos(screenPos)
	, cameraPosition(fromCameraToNode)
	{;}

	virtual bool needRecalculate(std::string nodeName){
		return true;
	}
protected:
	virtual void reorientCamera(){
		//1. Calculate new values
		Ogre::Matrix4 headPos=upperNode->_getFullTransform();
		//Use virtual position
		Ogre::Vector3 position=headPos.getTrans();
		//Orient camera towards the screen
		Ogre::Quaternion screenQuat=screenPos.extractQuaternion();
		Ogre::Quaternion cameraOrientation=screenQuat*Ogre::Quaternion(Ogre::Radian(3.14f),Ogre::Vector3(0,1,0));
		//Store the position of the camera
		cameraPosition.makeTransform(position,Ogre::Vector3(1,1,1),cameraOrientation);
		//...we have to express it in node's coordinates:
		cameraPosition=upperNode->getParentSceneNode()->_getFullTransform().inverse()*cameraPosition;
		//2. Apply new values		
		Ogre::Vector4 pos=cameraPosition*Ogre::Vector4(0,0,0,1);
		upperNode->setPosition(pos[0],pos[1],pos[2]);
		upperNode->setOrientation(cameraPosition.extractQuaternion());
		upperNode->setScale(Ogre::Vector3(1,1,1));
	}

	virtual void recalculateFrustum(){
		//1. Calculate new values:
		this->calculateProjectionData(upperNode->_getFullTransform(),screenPos,width, height,&FOVy,&x_offset,&y_offset);
		//2. Apply new values
		camera->setAspectRatio(aspectRatio);
		camera->setFOVy(Ogre::Radian(FOVy));
		camera->setFrustumOffset(-x_offset,-y_offset);

	}

	virtual void update(){
		//Set the position/orientation/projection volume of the camera.
		reorientCamera();
		recalculateFrustum();
		camera->setVisible(false);
	}
	
	void setScreenPos(Ogre::Matrix4 screenVW){
		screenPos=screenVW;
	}	

};

}
#endif
