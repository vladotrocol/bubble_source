#ifndef _CAMERA_MANAGER
#define _CAMERA_MANAGER
#include "Ogre.h"
#include <boost/shared_ptr.hpp>

class FieldData;

namespace OgreUtils{

class CameraData;
class ViewportData;
class AttachPointData;

//typedef boost::shared_prt<CameraManager> pCameraManager ;

class CameraManager{
	std::list<CameraData*> allCameras;
	std::list<CameraData*> pendingCameras;
	std::list<ViewportData*> allViewports;
	std::map<std::string,std::list<ViewportData*>> cameraViewports;
	CameraManager();
public:
	static boost::shared_ptr<CameraManager> instance();
	void init();
	bool addCamera(CameraData* cam);
	bool addViewport(ViewportData* vp);
	/**
	Check if a newly inserted node is the AttachPoint of any our cameras. 
	In that case the camera is created and any associated viewports are initialyzed. 
	*/
	bool checkNode(Ogre::SceneNode* node,Ogre::SceneManager* mgr);
	inline bool isMainCamera(Ogre::SceneNode* node,Ogre::SceneManager* mgr);
	/**
	Makes all the cameras to update their position/orientation/projection volumes. 
	If it is called every frame, it is an easy way to assure all the cameras are up-to-date. 
	Can be too consuming if there are many viewports.
	*/
	void updateAllCameras();
	/**
	Returns the camera with name cameraName
	*/
	CameraData* getCamera(std::string cameraName);
	/**
	Returns the ViewportData with name viewportName.
	*/
	ViewportData* getViewport(std::string viewportName);
	/**
	Parses all the subfields looking for Camera and Viewport settings.
	*/
	void readConfig(FieldData* configuration);
	void reset();
protected:

	/**
	This method is called when a camera is being initialized. It sets up all the viewports associated to the camera.
	*/
	void bindCameraToViewports(CameraData* cam);
	void readHMDCamera(FieldData* f);
	void readProjectionCamera(FieldData* f);
	void readDynamicCamera(FieldData* f);	
	void readViewport(FieldData* f,int z_order);
	AttachPointData readAttachPoint(FieldData* f);
};
}
#endif
