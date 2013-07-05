#ifndef _COMPOSITE_OGRE_APPLICATION_H
#define _COMPOSITE_OGRE_APPLICATION_H
#include <./DisplayKernel/OgreApplications/IOgreApplication.h>
#include <vector>


class CompositeOgreApplication:public IOgreApplication , public Ogre::WindowEventListener{
	std::vector<IOgreApplication*> applications;
public:
    CompositeOgreApplication(void);
    virtual void go(void);
	virtual void addOgreApplication(IOgreApplication* o);

    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void); // Override me!
    virtual void destroyScene(void);
    virtual int createViewports(int curIndex=0);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
protected: 
    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    // OgreBites
    bool mShutDown;
};
#endif