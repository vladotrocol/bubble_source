#include "./DisplayKernel/OgreApplications/LayerSimulation.h"
#include "./DisplayKernel/OgreUtils/CameraUtils.h"


using namespace OgreUtils;

//-------------------------------------------------------------------------------------
LayerApplication::LayerApplication(WaterLayerInfo& wl, std::string LayerCGMaterial1_16="LayerCG1_16", std::string LayerCGMaterial17_31="LayerCG17_31", int OIS_black_code=OIS::KC_NUMPAD0)
	: layerData(wl)	
	, layerCGMaterial1_16(LayerCGMaterial1_16)
	, layerCGMaterial17_31(LayerCGMaterial17_31)
	, numFrames(0)
	, firstTime(true)
	, OIS_black_code(OIS_black_code)
	, isBlack(false)
{
	;
}
//-------------------------------------------------------------------------------------
LayerApplication::~LayerApplication(void)
{
}
//-------------------------------------------------------------------------------------

void LayerApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}

void LayerApplication::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("mustardOutput");
    mCamera->setPosition(Ogre::Vector3(0,0,0));
    mCamera->lookAt(Ogre::Vector3(0,0,1));
    mCamera->setNearClipDistance(1);
    mCamera->setFrustumExtents(-0.5,0.5,0.5,-0.5);
	mCamera->setVisible(true);
}

int LayerApplication::createViewports(int curIndex)
{
     // Create one viewport, entire window
    //Ogre::Viewport* LayerProjector = mWindow->addViewport(mCamera,curIndex,(curIndex%3)/3.0,(curIndex>3?0:0.5),0.33f,0.33);
	//Ogre::Viewport* LayerProjector = mWindow->addViewport(mCamera,curIndex,0,0,1,1);
    //LayerProjector->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));

    // Alter the camera aspect ratio to match the viewport
    //mCamera->setAspectRatio(
   //     Ogre::Real(LayerProjector->getActualWidth()) / Ogre::Real(LayerProjector->getActualHeight()));
	return ++curIndex;
	return curIndex;
}

void LayerApplication::createScene(void)
{
	{//1. Configure a Background (DEBUG)
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);
	}
	{
	Ogre::SceneNode* node= mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0,0,-1);
	node->attachObject(mCamera);
	CameraUtils::configureRtt(layerData.outputTexture, layerData.px_width, layerData.px_height, mCamera, this);
	//ptr->getBuffer()->getRenderTarget()->setAutoUpdated(false);
	
	}
	{ //3. Create two planes: Border and data with the a texture attached to it
		float A0=layerData.dstPoints[0][0]/layerData.px_width; float A1= layerData.dstPoints[0][1]/layerData.px_height;
		float B0=layerData.dstPoints[1][0]/layerData.px_width; float B1= layerData.dstPoints[1][1]/layerData.px_height;
		float C0=layerData.dstPoints[2][0]/layerData.px_width; float C1= layerData.dstPoints[2][1]/layerData.px_height;
		float D0=layerData.dstPoints[3][0]/layerData.px_width; float D1= layerData.dstPoints[3][1]/layerData.px_height;
		printf("A(%f,%f) ; B(%f,%f) ; C(%f,%f) ; D(%f,%f)\n", A0,A1,B0,B1,C0,C1,D0,D1);
	//3.1. The Plane node which contains the Plane entities 
    Ogre::ManualObject* layerPolygon=mSceneMgr->createManualObject("leftEyeManualObject");
	layerPolygon->setDynamic(true);
	std::string aux_str("leftEyeManualObjectMat");
	layerPolygon->begin(aux_str);
	layerPolygon->position(A0-0.5,  0.5- A1,0);layerPolygon->textureCoord(0,0);
	layerPolygon->position(D0-0.5,  0.5- D1,0);layerPolygon->textureCoord(0,1);
	layerPolygon->position(C0-0.5,  0.5- C1,0);layerPolygon->textureCoord(1,1);
	layerPolygon->end();
	layerPolygon->begin(aux_str);
	layerPolygon->position(C0-0.5,  0.5- C1,0);layerPolygon->textureCoord(1,1);
	layerPolygon->position(B0-0.5,  0.5- B1,0);layerPolygon->textureCoord(1,0);
	layerPolygon->position(A0-0.5,  0.5- A1,0);layerPolygon->textureCoord(0,0);
	layerPolygon->end();
	
	/*layerPolygon->begin(aux_str);
	layerPolygon->position(-0.5,  0.5,0);layerPolygon->textureCoord(layerData.srcPoints[0][0],layerData.srcPoints[0][1]);
	layerPolygon->position(-0.5, -0.5,0);layerPolygon->textureCoord(layerData.srcPoints[1][0],layerData.srcPoints[1][1]);
	layerPolygon->position(0.5,  -0.5,0);layerPolygon->textureCoord(layerData.srcPoints[2][0],layerData.srcPoints[2][1]);
	layerPolygon->end();
	layerPolygon->begin(aux_str);
	layerPolygon->position(0.5, -0.5,0);layerPolygon->textureCoord(layerData.srcPoints[2][0],layerData.srcPoints[2][1]);
	layerPolygon->position(0.5, 0.5,0);layerPolygon->textureCoord(layerData.srcPoints[3][0],layerData.srcPoints[3][1]);
	layerPolygon->position(-0.5, 0.5,0);layerPolygon->textureCoord(layerData.srcPoints[0][0],layerData.srcPoints[0][1]);
	layerPolygon->end();*/
	//1.b) Apply the material to the plane
	layerPolygon->setMaterialName(0,this->layerCGMaterial1_16);
	layerPolygon->setMaterialName(1,this->layerCGMaterial1_16);
	//layerPolygon->setMa
	Ogre::SceneNode* layerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    layerNode->attachObject(layerPolygon);
	layerPolygon->getSection(0)->setCustomParameter(FPC_CG_index,Ogre::Vector4(layerData.framesPerCycle,layerData.framesPerCycle,layerData.framesPerCycle,layerData.framesPerCycle));
	layerPolygon->getSection(1)->setCustomParameter(FPC_CG_index,Ogre::Vector4(layerData.framesPerCycle,layerData.framesPerCycle,layerData.framesPerCycle,layerData.framesPerCycle));
	}
}

bool LayerApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//ptr->getBuffer()->getRenderTarget()->update();
	//Update the frustum of the camera.
	if(numFrames<10){
		numFrames++;
		return true;
	}
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(this->layerCGMaterial1_16);
	material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(layerData.inputTexture);
	int i=0;
	int end1=layerData.framesPerCycle<=15?layerData.framesPerCycle:15;
	int end2=layerData.framesPerCycle<=15?0:layerData.framesPerCycle;
	for(;i<end1;i++)
		material->getTechnique(0)->getPass(0)->getTextureUnitState(i+1)->setTextureName(layerData.stencil[layerData.curCycle][i]);
	for(;i<end2;i++)
		material->getTechnique(0)->getPass(1)->getTextureUnitState(i-15)->setTextureName(layerData.stencil[layerData.curCycle][i]);
	return BaseApplication::frameRenderingQueued(evt);	
	
}

/*void LayerApplication::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt){
		const int measureOverNSeconds=5; //We print it over 5 seconds....
		float curSecond;
		float curTime=((float)(timer.getMicroseconds()+1)/1000000);
		//0. Initialize clock
		if(firstTime){
			timer.reset();
			cyclesSinceLastSecond=0;
			lastSecond=curSecond=1.0f*static_cast<int>(curTime);
			firstTime=false;
		}
		
		//1. Update number of images and cycles since last second
		cyclesSinceLastSecond++;
		//2. Determine in the second is finished and notify if necessary
		curSecond=static_cast<int>(curTime);
		if(curSecond>=lastSecond+measureOverNSeconds){
			printf("FPS_Layer->%f",(cyclesSinceLastSecond/(curSecond-lastSecond)));
			cyclesSinceLastSecond=0;lastSecond=curSecond;
		}	
			
}*/

/*bool LayerApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if(arg.key==OIS_black_code)
		if(isBlack){
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(this->layerCGMaterial1_16);
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(layerData.inputTexture);
			isBlack=false;
		}
		else{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(this->layerCGMaterial1_16);
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("black.png");
			isBlack=true;
		}
	return true;
}*/