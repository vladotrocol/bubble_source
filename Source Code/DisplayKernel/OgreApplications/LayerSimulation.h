#ifndef _LAYER_APPLICATION_H
#define _LAYER_APPLICATION_H
 
#include "./DisplayKernel/OgreApplications/BaseApplication.h"
#include "./DisplayKernel/OgreUtils/CameraOISController.h"
#include "./WaterDisplay/WaterLayerInfo.h"

class LayerApplication : public BaseApplication, Ogre::RenderTargetListener
{
public:
	static const int FPC_CG_index=123;
    LayerApplication(WaterLayerInfo& wl, std::string layerCGMaterial1_16, std::string layerCGMaterial17_31, int OIS_black_code);
    virtual ~LayerApplication(void);
 
	Ogre::Timer timer;
	bool firstTime;
	float lastSecond;
	int cyclesSinceLastSecond;
	
protected:
    virtual void createScene(void);
	virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual int createViewports(int curIndex=0);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	//virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	//virtual bool keyPressed( const OIS::KeyEvent &arg );
private:
	WaterLayerInfo& layerData;
	std::string layerCGMaterial1_16, layerCGMaterial17_31;
	void updateMask();
	int numFrames;
	int OIS_black_code;//Key that, when pressed, causes the application to display black
	bool isBlack;
	//Ogre::TexturePtr ptr;
	
};
 
#endif