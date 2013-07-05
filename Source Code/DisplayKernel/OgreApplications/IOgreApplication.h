#ifndef _IOGRE_APPLICATION_H
#define _IOGRE_APPLICATION_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <Ogre.h>
#include <OgreFontManager.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>


class IOgreApplication{
public:
    IOgreApplication(void);
    virtual ~IOgreApplication(void);
    virtual void go(void)=0;

    virtual bool setup()=0;
    virtual bool configure(void)=0;
	virtual void _setConfiguration(Ogre::Root* r,Ogre::RenderWindow* rw, OIS::InputManager* mInputManager, OIS::Mouse* mMouse, OIS::Keyboard* mKeyboard){
		this->mRoot=r;
		this->mWindow=rw;
		this->mInputManager=mInputManager;
		this->mMouse=mMouse;
		this->mKeyboard=mKeyboard;
		leader=false;//We are not creating the resources, somebody did this for us--> We will not destroy them
	}
    virtual void chooseSceneManager(void)=0;
    virtual void createCamera(void)=0;
    virtual void createFrameListener(void)=0;
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void)=0;
    virtual int createViewports(int curIndex=0)=0;
    virtual void setupResources(void)=0;
    virtual void createResourceListener(void)=0;
    virtual void loadResources(void)=0;

protected:
	bool leader;//This attribute determines wether this object has to destroy the Ogre resources (mRoot, mWindow...)    
	Ogre::Root *mRoot;
	Ogre::RenderWindow* mWindow;
	//OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

};

#endif // #ifndef __BaseApplication_h_
