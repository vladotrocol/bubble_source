#include <./DisplayKernel/OgreApplications/CompositeOgreApplication.h>


CompositeOgreApplication::CompositeOgreApplication(void)
    :mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mShutDown(false)
{
	applications.reserve(5);
}

void CompositeOgreApplication::addOgreApplication(IOgreApplication* o)
{
	applications.push_back(o);	
}

//-------------------------------------------------------------------------------------
bool CompositeOgreApplication::configure(void)
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
		for(unsigned int i=0;i<applications.size();i++)
			applications[i]->_setConfiguration(mRoot,mWindow,mInputManager,mMouse,mKeyboard);		
		return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::chooseSceneManager(void)
{
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->chooseSceneManager();		
}

void CompositeOgreApplication::createScene(void)
{
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->createScene();
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::createCamera(void)
{
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->createCamera();		;
}
//-------------------------------------------------------------------------------------

void CompositeOgreApplication::createFrameListener(void)
{
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->createFrameListener();		
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::destroyScene(void)
{
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->destroyScene();		
}
//-------------------------------------------------------------------------------------
int CompositeOgreApplication::createViewports(int curIndex)
{
    for(unsigned int i=0;i<applications.size();i++)
			curIndex=applications[i]->createViewports(curIndex);		
	return curIndex;
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::setupResources(void)
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
void CompositeOgreApplication::createResourceListener(void)
{
	for(unsigned int i=0;i<applications.size();i++)
			applications[i]->createResourceListener();		
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void CompositeOgreApplication::go(void)
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
bool CompositeOgreApplication::setup(void)
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
}


//Adjust mouse clipping area
void CompositeOgreApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void CompositeOgreApplication::windowClosed(Ogre::RenderWindow* rw)
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