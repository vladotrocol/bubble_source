#ifndef __MatrixRTTApplication_h_
#define __MatrixRTTApplication_h_
 
#include "./DisplayKernel/OgreApplications/BaseApplication.h"
#include "./DisplayKernel/OgreUtils/CameraOISController.h"


class SimulationManager;

class RTTApplication : public BaseApplication, Ogre::RenderTargetListener
{
public:
    RTTApplication(SimulationManager* simulation,float x_offset=0, float viewportWidthToUse=1, bool visualDebug=true);
    virtual ~RTTApplication(void);
 
protected:
    virtual void createScene(void);
	virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual int createViewports(int curIndex=0);
	//RenderTargetListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	// OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


private:
		
	//Setting both eyes
	Ogre::Camera *leftEyeCamera, *rightEyeCamera;
	Ogre::Entity*       modelEnt;
    Ogre::SceneNode*    modelNode;
	float flingVirtualWidth, flingVirtualHeight;
	int flingPixelWidth, flingPixelHeight;
	float zoom;
	//	All these elements are required to get a visual output of the system.
	bool visualDebug;
	float x_offset;
	float viewportWidth;
	SceneNodeController* camControl;
	void createTablePlane(float sizex, float sizey, float offsetz);
	void createAxis();
	Ogre::Viewport*  flingOutput;
	Ogre::SceneNode* mPlaneNode; 
	Ogre::Entity* mPlaneEnt;
	std::vector<Ogre::SceneNode*> miniScreens; 
	bool miniScreensVisible;
	SimulationManager* simulationManager;
	Ogre::Rectangle2D* conflictResolution;
};
 
#endif // #ifndef __MatrixRTTApplication_h_