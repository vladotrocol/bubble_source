#include "RTTApplication.h"
#include "./CameraManager/CameraManager.h"
#include "./CameraManager/ViewportData.h"
#include "./CameraManager/CameraData.h"
#include "./CameraManager/CameraUtils.h"
#include "SimulationManager.h"
#include "IConfigManager.h"
//#define SEE_MINISCREENS	


//-------------------------------------------------------------------------------------
RTTApplication::RTTApplication(SimulationManager* simulation,float x, float viewportWidthToUse, bool visDebug)
	: x_offset(x)
	, viewportWidth(viewportWidthToUse)
	, visualDebug(visDebug)
	, flingVirtualWidth(1)
	, flingVirtualHeight(0.586f)
	, flingPixelWidth(1024)
	, flingPixelHeight(600)
	, zoom(1)
	, simulationManager(simulation)
	, miniScreensVisible(true)

{
	camControl=SceneNodeController::instance();
}
//-------------------------------------------------------------------------------------
RTTApplication::~RTTApplication(void)
{
}
//-------------------------------------------------------------------------------------

void RTTApplication::chooseSceneManager(void)
{

    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	


}
int RTTApplication::createViewports(int curIndex)
{
	// Create one viewport, entire window
    if(visualDebug){
		flingOutput = mWindow->addViewport(mCamera,curIndex,x_offset,0.0f,viewportWidth,1);
		flingOutput->setBackgroundColour(Ogre::ColourValue(0.3f,0.3f,0.3f));
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(Ogre::Real(flingOutput->getActualWidth()) / Ogre::Real(flingOutput->getActualHeight()));
	}
	return ++curIndex;
}

void RTTApplication::createCamera(void)
{
    if(visualDebug){
		// Create the Default camera
		mCamera = mSceneMgr->createCamera("usr2Table");
		//mCamera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
		mCamera->setPosition(Ogre::Vector3(0,0,0));
		mCamera->roll(Ogre::Radian(3.1415f));
		mCamera->lookAt(Ogre::Vector3(0,0,1));
		mCamera->setNearClipDistance(0.25f);
	}
	//Create our cameras
	leftEyeCamera=mSceneMgr->createCamera("usr1LeftCamera");	
	leftEyeCamera->setNearClipDistance(1.0f);
	leftEyeCamera->lookAt(0,0,1);
	leftEyeCamera->setDebugDisplayEnabled(true);
    rightEyeCamera = mSceneMgr->createCamera("usr1RightCamera");
    rightEyeCamera->setNearClipDistance(1.0f);
	rightEyeCamera->lookAt(0,0,1);
	rightEyeCamera->setDebugDisplayEnabled(true);
}

void RTTApplication::createScene(void)
{
	InputManager* im=InputManager::instance();
	static_cast<AnalogInputSensor*>(im->getSensorByName("flingWidth"))->readData(&flingVirtualWidth);
	static_cast<AnalogInputSensor*>(im->getSensorByName("flingHeight"))->readData(&flingVirtualHeight);
	static_cast<DigitalInputSensor*>(im->getSensorByName("flingPixelWidth"))->readData(&flingPixelWidth);
	static_cast<DigitalInputSensor*>(im->getSensorByName("flingPixelHeight"))->readData(&flingPixelHeight);

    //light->setPosition(20, 80, 50);
	if(visualDebug){
		Ogre::SceneNode* node= mSceneMgr->getRootSceneNode()->createChildSceneNode("debugObserver");
		node->setOrientation(Ogre::Quaternion(Ogre::Radian(3.1415f),Ogre::Vector3( 0,1,0)));
		node->attachObject(mCamera);
		Ogre::Light* light = mSceneMgr->createLight("MainLight");
		light->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f,0.5f));
		light->setType(Ogre::Light::LT_POINT);
		light->setDirection(Ogre::Vector3(0,0,1));
		light->setAttenuation(3,0,1,0);
		light->setCastShadows(true);
		node->attachObject(light);
		camControl->setTarget(node);
		
		//createAxis();
		
	}

	{


	Ogre::SceneNode* pivot= mSceneMgr->getRootSceneNode()->createChildSceneNode("pipesPivot");
	float tableWidth, tableHeight;
	static_cast<AnalogInputSensor*>(im->getSensorByName("tableWidth"))->readData(&tableWidth);
	static_cast<AnalogInputSensor*>(im->getSensorByName("tableHeight"))->readData(&tableHeight);	
	simulationManager->createScene(mSceneMgr,pivot);//These values make pipes to be aligned to the axes 3D world
	
	//Updates cameras:
	// a) Left camera
	Ogre::SceneNode* eye = mSceneMgr->getRootSceneNode()->createChildSceneNode("usr1LeftNode");
	//eye = mSceneMgr->getSceneNode("debugObserver");//DEBUG, THE CAMERA IS ATTACHED TO THE OBSERVER'S VIEW
	eye->setPosition(-0.15f,0,5);
	Ogre::SceneNode* eyePivot = eye->createChildSceneNode("usr1LeftNodePivot");
	eyePivot->attachObject(leftEyeCamera);
	if(visualDebug){
		eyePivot->attachObject(mSceneMgr->createEntity("leftEyeDebug", "Centinel.mesh"));
		float SCALE=0.01f;
		Ogre::Entity* lookAtRay = mSceneMgr->createEntity("cube.mesh");
		Ogre::SceneNode* node = eye->createChildSceneNode();
		node->scale(0.01f*SCALE,SCALE*0.01f,SCALE*100);
		lookAtRay->setMaterialName("Examples/Red");
		node->attachObject(lookAtRay);

	
	}
	// b) Right camera
	eye = mSceneMgr->getRootSceneNode()->createChildSceneNode("usr1RightNode");
	eye->setPosition(0.15f,0,5);
	eyePivot = eye->createChildSceneNode("usr1RightNodePivot");
	eyePivot->attachObject(rightEyeCamera);
	if(visualDebug){
		float SCALE=0.01f;
		eyePivot->attachObject(mSceneMgr->createEntity("rightEyeDebug", "Centinel.mesh"));
		Ogre::Entity* lookAtRay = mSceneMgr->createEntity("cube.mesh");
		Ogre::SceneNode* node = eye->createChildSceneNode();
		node->scale(0.01f*SCALE,SCALE*0.01f,SCALE*100);
		lookAtRay->setMaterialName("Examples/Green");
		node->attachObject(lookAtRay);

	}

	//3. =============  RTT code  ==================
	CameraUtils::configureRtt("leftEyeTex", flingPixelWidth, flingPixelHeight,leftEyeCamera,this);
	CameraUtils::configureRtt("rightEyeTex", flingPixelWidth, flingPixelHeight,rightEyeCamera,this);
	
	if(visualDebug){
		//4. Create a material out of the texture:
		CameraUtils::createMaterialFromTexture("leftEyeTexMat","leftEyeTex");	
		//5. Create a plane with the RTT texture attached to it
		//a. The plane Entity
		Ogre::MovablePlane* mPlane = new Ogre::MovablePlane("Plane");
		mPlane->d = 0;
		mPlane->normal = Ogre::Vector3::UNIT_Z;
		Ogre::MeshManager::getSingleton().createPlane("PlaneMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, this->flingVirtualWidth ,this->flingVirtualHeight, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
		mPlaneEnt = mSceneMgr->createEntity("PlaneEntity", "PlaneMesh");
		//1.b) Apply the material to the plane
		mPlaneEnt->setMaterialName("leftEyeTexMat");
		//2. The Plane node with the Plane entity attached to it
		mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mPlaneNode->attachObject(mPlaneEnt);
		PositionInputSensor* fling1=dynamic_cast<PositionInputSensor*>(im->getSensorByName("fling1"));
		Ogre::Matrix4 fling1Pos;
		fling1->readData(&fling1Pos);
		mPlaneNode->setPosition(fling1Pos.getTrans());
		mPlaneNode->setOrientation(fling1Pos.extractQuaternion());
	}


	}


}
//-------------------------------------------------------------------------------------
bool RTTApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	this->simulationManager->update(evt.timeSinceLastFrame);
    //mPlaneNode->yaw(Ogre::Radian(evt.timeSinceLastFrame));
	if(visualDebug)
		camControl->updateNodePosition(evt.timeSinceLastFrame);
	static PositionInputSensor* usr1Left=NULL, *usr1Right=NULL, *fling1=NULL;
	static bool allGood=false;
	if(!allGood){
		if(!usr1Left)usr1Left=dynamic_cast<PositionInputSensor*>(InputManager::instance()->getSensorByName("usr1Left"));
		if(!usr1Right)usr1Right=dynamic_cast<PositionInputSensor*>(InputManager::instance()->getSensorByName("usr1Right"));
		if(!fling1)fling1=dynamic_cast<PositionInputSensor*>(InputManager::instance()->getSensorByName("fling1"));
		allGood=(usr1Left&&usr1Right&&fling1);
		return BaseApplication::frameRenderingQueued(evt);
	}
	//Update cameras positions
	Ogre::Matrix4 leftPos, rightPos, fling1Pos;
	usr1Left->readData(&leftPos);
	usr1Right->readData(&rightPos);
	fling1->readData(&fling1Pos);
	Ogre::SceneNode* l=mSceneMgr->getSceneNode("usr1LeftNode"), *r=mSceneMgr->getSceneNode("usr1RightNode");
	l->setOrientation(leftPos.extractQuaternion());
	l->setPosition(leftPos.getTrans());
	r->setOrientation(rightPos.extractQuaternion());
	r->setPosition(rightPos.getTrans());	
	//Update camera frustum
	CameraUtils::updateCameraFrustum(leftEyeCamera,mSceneMgr->getSceneNode("usr1LeftNodePivot"),fling1Pos, flingVirtualWidth/zoom,flingVirtualHeight/zoom);
	CameraUtils::updateCameraFrustum(rightEyeCamera,mSceneMgr->getSceneNode("usr1RightNodePivot"),fling1Pos, flingVirtualWidth/zoom,flingVirtualHeight/zoom);
	if(visualDebug){
		mPlaneNode->setPosition(fling1Pos.getTrans());
		mPlaneNode->setOrientation(fling1Pos.extractQuaternion());
		static int i=0;
		i++;
#ifdef SEE_MINISCREENS			
		if(i==15){
			float x=0.05f;
			float y=0.05f;
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.0f,  y+0.0f, 0.175f, 0.3f,"leftEyeTexMat",mSceneMgr->getRootSceneNode(),&conflictResolution));
			CameraUtils::createMaterialFromTexture("rightEyeTexMat","rightEyeTex");
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.0f,  0.7f -y, 0.175f, 0.3f,"rightEyeTexMat",mSceneMgr->getRootSceneNode(),&conflictResolution));
			CameraUtils::createMaterialFromTexture("leftEyeMaskMat","leftEyeMask");
			CameraUtils::createMaterialFromTexture("rightEyeMaskMat","rightEyeMask");
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.2f,  y+0.00f, 0.175f, 0.3f,"leftEyeMaskMat",mSceneMgr->getRootSceneNode(),&conflictResolution));
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.2f,  0.70f -y, 0.175f, 0.3f,"rightEyeMaskMat",mSceneMgr->getRootSceneNode(),&conflictResolution));
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.4f,y+0.0f,0.175f,0.3f,"MustardProjectionLeft",mSceneMgr->getRootSceneNode(),&conflictResolution));
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.4f, 0.7f -y,0.175f,0.3f,"MustardProjectionRight",mSceneMgr->getRootSceneNode(),&conflictResolution));
			miniScreens.push_back(CameraUtils::createMiniScreen(x+0.875f, 0.65f,-0.175f,-0.30f,IConfigManager::instance()->getConflictResolutionMaterial(),mSceneMgr->getRootSceneNode(),&conflictResolution));
			
		}
		else 
#endif		
		if(i>10)
			conflictResolution->setMaterial(IConfigManager::instance()->getConflictResolutionMaterial());
		//mPlaneEnt->setMaterialName("leftEyeMask");
	}
	/*static const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
	static int frame=0;
	frame=(++frame)%300;
	if(frame==0)
		printf("\n FPS-> %.1f, AvFPS-> %.1f", stats.lastFPS, stats.avgFPS);
	*/
    return BaseApplication::frameRenderingQueued(evt);
}

//---------What to do before and after the Miniscreen are going to be rendered----------------------------------------------------------------------------
void RTTApplication::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{	
	if(visualDebug && miniScreensVisible){
		mPlaneNode->setVisible(false);
#ifdef SEE_MINISCREENS			
		for(size_t i=0;i<miniScreens.size();i++)
			miniScreens[i]->setVisible(false);
#endif
	}
}
//-------------------------------------------------------------------------------------
void RTTApplication::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    if(visualDebug && miniScreensVisible){
		mPlaneNode->setVisible(true);
#ifdef SEE_MINISCREENS			
		for(size_t i=0;i<miniScreens.size();i++)
			miniScreens[i]->setVisible(true);
#endif
	}
}

//-------------------------------------------------------------------------------------
static bool pressedSpace=false;

bool RTTApplication::keyPressed( const OIS::KeyEvent &arg )
{
{	//PART A: Control the application itself
	this->simulationManager->keyPressed(arg);
	if(visualDebug){
		camControl->injectKeyDown(arg);
		if(arg.key == OIS::KC_1)   // go to left eye camera
		{
			Ogre::SceneNode* observer=mSceneMgr->getSceneNode("debugObserver");
			Ogre::SceneNode* leftEye=mSceneMgr->getSceneNode("usr1LeftNode");
			//observer->setOrientation(leftEye->getOrientation());
			observer->setPosition(leftEye->getPosition());
		}
		if(arg.key == OIS::KC_2)   
		{
			Ogre::SceneNode* observer=mSceneMgr->getSceneNode("debugObserver");
			Ogre::SceneNode* leftEye=mSceneMgr->getSceneNode("usr1RightNode");
			//observer->setOrientation(leftEye->getOrientation());
			observer->setPosition(leftEye->getPosition());
		}
#ifdef SEE_MINISCREENS			
		if(arg.key == OIS::KC_3)   
		{
			static int step=0;
			static const int numSteps=3;
			if(step<numSteps){
				miniScreens[2*step]->flipVisibility();
				miniScreens[2*step+1]->flipVisibility();
			}
			if(step==numSteps)
				miniScreens[2*step]->flipVisibility();
			if(step>numSteps){
				for(size_t i=0;i<miniScreens.size();i++)
					miniScreens[i]->flipVisibility();	
				step=0;
			}
			step++;
			//miniScreensVisible=!miniScreensVisible;
		}
#endif	
		static bool showMainWindow=true;
		if(arg.key == OIS::KC_4){
			showMainWindow=!showMainWindow;
			if(!showMainWindow)
				flingOutput->setDimensions(0,0,0.11,0.20);
			else
				flingOutput->setDimensions(0,0,this->viewportWidth,1);
		}

	}
}
	//PART B: Settings to control the mode of operation (mono, paralax, RHM, frecuency, on/off...)
{	static float fast=false;
	static float callibrate=false;
	static IConfigManager* mm=IConfigManager::instance();
	static int fastRate=4;

    if(arg.key == OIS::KC_0)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
		Ogre::MaterialManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
		return false;
    }
	else if((arg.key == OIS::KC_7)&&!pressedSpace){
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
	}
}
    //mCameraMan->injectKeyDown(arg);
    return true;
}

bool RTTApplication::keyReleased( const OIS::KeyEvent &arg )
{
	this->simulationManager->keyReleased(arg);
	if(visualDebug&& camControl)
		camControl->injectKeyUp(arg);
	if((arg.key == OIS::KC_5 || arg.key == OIS::KC_6)&&pressedSpace){
			pressedSpace=false;			
	}
	//mCameraMan->injectKeyUp(arg);
    return true;
}

/*bool RTTApplication::keyPressed( const OIS::KeyEvent &arg )
{	
	BaseApplication::keyPressed(arg);
	this->simulationManager->keyPressed(arg);
	if(visualDebug){
		camControl->injectKeyDown(arg);
		if(arg.key == OIS::KC_1)   // go to left eye camera
		{
			Ogre::SceneNode* observer=mSceneMgr->getSceneNode("debugObserver");
			Ogre::SceneNode* leftEye=mSceneMgr->getSceneNode("usr1LeftNode");
			//observer->setOrientation(leftEye->getOrientation());
			observer->setPosition(leftEye->getPosition());
		}
		if(arg.key == OIS::KC_2)   
		{
			Ogre::SceneNode* observer=mSceneMgr->getSceneNode("debugObserver");
			Ogre::SceneNode* leftEye=mSceneMgr->getSceneNode("usr1RightNode");
			//observer->setOrientation(leftEye->getOrientation());
			observer->setPosition(leftEye->getPosition());
		}
#ifdef SEE_MINISCREENS			
		if(arg.key == OIS::KC_3)   
		{
			static int step=0;
			static const int numSteps=3;
			if(step<numSteps){
				miniScreens[2*step]->flipVisibility();
				miniScreens[2*step+1]->flipVisibility();
			}
			if(step==numSteps)
				miniScreens[2*step]->flipVisibility();
			if(step>numSteps){
				for(size_t i=0;i<miniScreens.size();i++)
					miniScreens[i]->flipVisibility();	
				step=0;
			}
			step++;
			//miniScreensVisible=!miniScreensVisible;
		}
#endif	
		static bool showMainWindow=true;
		if(arg.key == OIS::KC_4){
			showMainWindow=!showMainWindow;
			if(!showMainWindow)
				flingOutput->setDimensions(0,0,0.11,0.20);
			else
				flingOutput->setDimensions(0,0,this->viewportWidth,1);
		}

	}
	return true;
}

bool  RTTApplication::keyReleased( const OIS::KeyEvent &arg ){
	BaseApplication::keyReleased(arg);
	this->simulationManager->keyReleased(arg);
	if(visualDebug&& camControl)
		camControl->injectKeyUp(arg);
	return true;
}*/

bool  RTTApplication::mouseMoved( const OIS::MouseEvent &arg ){
	BaseApplication::mouseMoved(arg);
	this->simulationManager->mouseMoved(arg);
	if(visualDebug && camControl)
		camControl->injectMouseMove(arg);
	return true;
}

bool  RTTApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	BaseApplication::mousePressed(arg,id);
	this->simulationManager->mousePressed(arg,id);
	if(visualDebug && camControl)
		camControl->injectMouseDown(arg,id);
	return true;
}

bool  RTTApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	BaseApplication::mouseReleased(arg,id);
	this->simulationManager->mouseReleased(arg,id);
	if(visualDebug && camControl)
		camControl->injectMouseUp(arg,id);
	return true;
}


