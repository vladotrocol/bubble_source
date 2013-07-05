/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "./DisplayKernel/OgreApplications/BaseApplication.h"
//#include "CompositeConfigManager.h"     

//-------------------------------------------------------------------------------------
IOgreApplication::IOgreApplication()
	:mRoot(0),
	mWindow(0),
	mInputManager(0),
    mMouse(0),
    mKeyboard(0),
	leader(true)
{;}

IOgreApplication::~IOgreApplication(){

   if (leader && mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
			mWindow=NULL;
			delete mRoot;
		}
		
}

BaseApplication::BaseApplication()
    :mCamera(0),
    mSceneMgr(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mShutDown(false)
{
	;
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	//Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    
	// Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig())
    {
        // If returned true, user clicked OK so initialise
        Ogre::NameValuePairList misc;
		misc["monitorIndex"] = "1";  //I use two monitors, so I tell Ogre to draw on my main (#1) monitor
		misc["FullScreen"] = "No";  
		misc["FullScreen"] = "No";  
		misc["Colour Depth"]="32";
		misc["Display Frequency"]="60";
		// Here we tell the system not to create a default rendering window by passing 'false'
        mWindow = mRoot->initialise(false, "WaterDisplay2");
		//now we need to create a render window manually
		mWindow = mRoot->createRenderWindow("WaterDisplay2",1024,768,false, &misc ); //pass our custom attributes in "misc"
		mWindow->setVSyncEnabled(true);
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		/////////////////////////////////// NON EXCLUSIVE INPUT WITH OIS //////////////////////////////////////////
		// Source: http://www.ogre3d.org/tikiwiki/Using+OIS
		// insert the following lines right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
		#if defined OIS_WIN32_PLATFORM
		paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		#elif defined OIS_LINUX_PLATFORM
		paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		#endif
		mInputManager = OIS::InputManager::createInputSystem( paramList );
		////////////////////////////////////
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}


//-------------------------------------------------------------------------------------

void BaseApplication::createFrameListener(void)
{   
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
int BaseApplication::createViewports(int curIndex)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera,curIndex);
    vp->setBackgroundColour(Ogre::ColourValue(0.0f,0,0.15));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	return ++curIndex;
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    return true;
}
//-------------------------------------------------------------------------------------
static bool pressedSpace=false;

bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	static float fast=false;
	static float callibrate=false;
	
	static int fastRate=4;

    /*if(arg.key == OIS::KC_0)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
		Ogre::MaterialManager::getSingleton().reloadAll();
    }
    else */if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
		return false;
    }
	/*else if((arg.key == OIS::KC_7)&&!pressedSpace){
		fastRate=(--fastRate>0?fastRate:1);
		if(fast)
			mm->setRefreshRate(fastRate);
	}
	else if((arg.key == OIS::KC_8)&&!pressedSpace){
		fastRate++;
		if(fast)
			mm->setRefreshRate(fastRate);
	}
	else if((arg.key == OIS::KC_5)&&!pressedSpace){
			pressedSpace=true;
			fast=!fast;
			mm->setRefreshRate(fast?fastRate:100000);
		}
	else if((arg.key == OIS::KC_6)&&!pressedSpace){
		pressedSpace=true;
		callibrate=!callibrate;
		mm->nextMode();
	}*/
    //mCameraMan->injectKeyDown(arg);
    return false;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	if((arg.key == OIS::KC_5 || arg.key == OIS::KC_6)&&pressedSpace){
			pressedSpace=false;			
	}
    //mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    //if (mTrayMgr->injectMouseMove(arg)) return true;
    //mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    //if (mTrayMgr->injectMouseDown(arg, id)) return true;
    //mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    //if (mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);
    return true;
}


//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}



