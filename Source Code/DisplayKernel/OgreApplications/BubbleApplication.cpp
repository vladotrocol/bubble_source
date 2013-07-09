#include "./DisplayKernel/OgreApplications/BubbleApplication.h"
#include "BubbleState.h"
#include "./DisplayKernel/OgreUtils/Primitives.h"
void BubbleApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("mainCamera");

    // Position it at 500 in Z direction
	float dist=768*Ogre::Math::Cos(30*6.28f/360);
    mCamera->setPosition(Ogre::Vector3(0,0,-dist));
    // Look back along -Z
	mCamera->pitch(Ogre::Radian(3.14f));
    //mCamera->lookAt(Ogre::Vector3(0,0,0));
    //mCamera->setNearClipDistance(5);
	mCamera->setFOVy(Ogre::Radian(3.14f/3));//60 degrees vertical FOV.
}

void BubbleApplication::createScene(void)
{
	{//1. Configure a Background (DEBUG)
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);
	BaseApplication::createAxis(mSceneMgr->getRootSceneNode(), 1024, 5);
	//BaseApplication::createTablePlane(1024,768,0,mSceneMgr->getRootSceneNode());
	}
	//2. Load a sphere
	Primitives::createSphere("sampleBubble.mesh", 1, 8,8);


	Ogre::SceneNode* aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* auxEnt=mSceneMgr->createEntity("cube.mesh");
	auxEnt->setMaterialName("Examples/OgreLogo");
	aux->attachObject(auxEnt);
	float SCALE=0.002f;
	aux->scale(100*SCALE,SCALE*100,SCALE*100);	
	aux->setPosition(512,0,0);

	aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
	auxEnt=mSceneMgr->createEntity("cube.mesh");
	auxEnt->setMaterialName("Examples/BeachStones");
	aux->attachObject(auxEnt);
	aux->scale(100*SCALE,SCALE*100,SCALE*100);	
	aux->setPosition(0,384,0);



}


//This method is called every frame.
bool BubbleApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//1. Mark all our nodes as dirty... if they are not updated, they will be removed
	setAllNodesDirty();
	//2. Traverse bubbles, updating the position of our nodes, creating new nodes, etc...
	updateNodesPositions();
	//3. Remove dirty nodes (they poped or wathever. This is none of our business)
	removeDirtyNodes();



	return BaseApplication::frameRenderingQueued(evt);
}

void BubbleApplication::setAllNodesDirty(){
	std::map<unsigned int, _GraphicalBubble>::iterator it=graphicBubbles.begin();
	for(;it!=graphicBubbles.end();it++)
		it->second.dirty=true;
}

void BubbleApplication::removeDirtyNodes(){
	std::map<unsigned int, _GraphicalBubble>::iterator it=graphicBubbles.begin();
	while(it!=graphicBubbles.end())
		if(it->second.dirty)
			it=graphicBubbles.erase(it);
		else it++;
}


void BubbleApplication::updateNodesPositions(){
	static Ogre::SceneNode* aux=NULL;
	
	//0. Get current position of bubbles
	static BubbleState& bubbles=BubbleState::instance();
	std::map<unsigned int, Bubble>* curBubbles= bubbles.getCurrentState();
	std::map<unsigned int, Bubble>::iterator itBubbles=curBubbles->begin();
	//1. Traverse each bubble
	for(;itBubbles!=curBubbles->end();itBubbles++){
		//1.1. Try and get a handler of the graphical representation of this bubble
		std::map<unsigned int, _GraphicalBubble>::iterator it=graphicBubbles.find(itBubbles->first);
		//1.2. Check if it already existed...
		if(it!=graphicBubbles.end())
		{//This bubble already existed... we simply update its position
			it->second.node->setPosition(itBubbles->second.center.x-512,itBubbles->second.center.y-384, 0); //DIEGO: BUBBLES SHOULD NOT USE 2D COORDINATES!!!! Bubble::center should not be cv::Point2f!!!!!
			//it->second.node->setPosition(itBubbles->second.center.x,itBubbles->second.center.y, 0);
			it->second.dirty=false;
		}
		else{

			//We need to set up a whole new bubble
			aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("sampleBubble.mesh");
			auxEnt->setMaterialName("Examples/ColourCheckerBoard");//itBubbles->second.materialToShow
			aux->attachObject(auxEnt);
			//Set its size and position
			//aux->scale(itBubbles->second.radius,itBubbles->second.radius,-itBubbles->second.radius);	
			aux->scale(itBubbles->second.radius,-itBubbles->second.radius,itBubbles->second.radius);	
			aux->setPosition(itBubbles->second.center.x-512,itBubbles->second.center.y-384, 0); //DIEGO: BUBBLES SHOULD NOT USE 2D COORDINATES!!!! Bubble::center should not be cv::Point2f!!!!!
			//Store it
			_GraphicalBubble gb;
			gb.dirty=false;
			gb.node=aux;
			graphicBubbles[itBubbles->first]=gb;
			
		}
	}
	/*
	//Let's see what happens when fliping normals
	static int i=0;
	i=(i+1)%20;
	if(i==0)
		aux->scale(1,1,-1);	*/

}