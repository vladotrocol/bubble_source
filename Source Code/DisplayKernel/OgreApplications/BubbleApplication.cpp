#include "./DisplayKernel/OgreApplications/BubbleApplication.h"
#include "BubbleState.h"
#include "./DisplayKernel/OgreUtils/Primitives.h"
#include <map>
//2. Control parameters (Gathered measuring projector distance, image height and width...All in cm)
float Zdistance=1000;						//Distance between the projector and the wall
float screenWidth=1390, screenHeight=1040;	//Size of the image generated
float screenTop=1160, screenBottom=120;					//Bottom and top position of the image (relative to projector)
float screenLeft=-screenWidth/2, screenRight=screenWidth/2;
float centerX=(screenLeft+screenRight)/2;
float centerY=(screenTop+screenBottom)/2;

//------------------------------------------------Predictive Filter-----------------------------------------------
//static float myTime=0;
//struct mouse_info_struct { float x,y; float vx,vy, ax, ay; float time;};
//struct mouse_info_struct mouse_info = {-1,-1,0,0,0,0}, last_mouse;
//struct _predictor{ struct mouse_info_struct t_1, t_2, prediction;};
//map<unsigned int, _predictor> predictor;
//
//void assignFilterVals(int x, int y) {
//	{
//		last_mouse = mouse_info;
//		mouse_info.x = x;
//		mouse_info.y = y;
//		mouse_info.vx=x-last_mouse.x;
//		mouse_info.vy=y-last_mouse.y;
//		mouse_info.time=myTime;		
//	}
//}
//
//void initFilter(unsigned int id){
//	predictor[id].t_2=mouse_info;
//	predictor[id].t_2.time=0;
//	predictor[id].t_1=mouse_info;
//	predictor[id].t_1.time=0;
//	predictor[id].prediction=predictor[id].t_1;
//}
//
//float v_correction=0.2;
//float a_correction=0.2;
//void correctFilter(unsigned int id){
//	predictor[id].t_2=predictor[id].t_1;
//	predictor[id].t_1=mouse_info;
//	//Compute current velocity according to last two measures
//	predictor[id].t_1.vx=(predictor[id].t_1.x-predictor[id].prediction.x)/(myTime-predictor[id].t_2.time);
//	predictor[id].t_1.vy=(predictor[id].t_1.y-predictor[id].prediction.y)/(myTime-predictor[id].t_2.time);
//	//predictor[id].t_1.vx=(predictor[id].t_1.x-predictor[id].prediction.x);
//	//predictor[id].t_1.vy=(predictor[id].t_1.y-predictor[id].prediction.y);
//	if(predictor[id].t_1.vx==0)
//		predictor[id].t_1.time++;
//	else 
//		predictor[id].t_1.time++;
//	//Compute predicted velocity:
//	
//	predictor[id].prediction.vx=v_correction*predictor[id].t_1.vx + (1-v_correction)*predictor[id].prediction.vx;
//	predictor[id].prediction.vy=v_correction*predictor[id].t_1.vy + (1-v_correction)*predictor[id].prediction.vy;
//	// Correct acceleration from two last oberverd measures
//	//printf("%f%", predictor[id].prediction.vx);
//	predictor[id].prediction.ax=((predictor[id].t_1.vx-predictor[id].t_2.vx)*a_correction + (1-a_correction)*predictor[id].prediction.ax)/(myTime-predictor[id].t_2.time);
//	predictor[id].prediction.ay=((predictor[id].t_1.vy-predictor[id].t_2.vy)*a_correction + (1-a_correction)*predictor[id].prediction.ay)/(myTime-predictor[id].t_2.time);
//	//predictor[id].prediction.ax=((predictor[id].t_1.vx-predictor[id].t_2.vx)*a_correction + (1-a_correction)*predictor[id].prediction.ax);
//	//predictor[id].prediction.ay=((predictor[id].t_1.vy-predictor[id].t_2.vy)*a_correction + (1-a_correction)*predictor[id].prediction.ay);
//	
//	predictor[id].prediction.time=myTime;
//	myTime=0;
//}
//
//void predict(unsigned int id, double time){
//	double inc_t=(time-predictor[id].t_2.time);
//	predictor[id].prediction.x=predictor[id].prediction.x+predictor[id].prediction.vx*inc_t+predictor[id].prediction.ax*inc_t*inc_t/2;
//	predictor[id].prediction.y=predictor[id].prediction.y+predictor[id].prediction.vy*inc_t+predictor[id].prediction.ay*inc_t*inc_t/2;
//	//predictor[id].prediction.x=predictor[id].prediction.x+predictor[id].prediction.vx+predictor[id].prediction.ax;
//	//predictor[id].prediction.y=predictor[id].prediction.y+predictor[id].prediction.vy+predictor[id].prediction.ay;
//}
//------------------------------------------------------------------------------------------------------------------------------


void BubbleApplication::createCamera(void)
{

    // Create the camera
    mCamera = mSceneMgr->createCamera("mainCamera");
    // Look back along -Z
	mCamera->setPosition(Ogre::Vector3(0,0,0));
	//Compute Frustum related stuff
	Ogre::Radian FOVy=2*Ogre::Math::ATan(screenHeight/(2*Zdistance));
	Ogre::Radian FOVx=2*Ogre::Math::ATan(screenWidth/(2*Zdistance));
    float Xoffset=centerX/Zdistance;
	float Yoffset=centerY/Zdistance;
	//Apply Frustum related stuff
	mCamera->setAspectRatio(screenWidth/screenHeight);
    mCamera->setFOVy(FOVy);//60 degrees vertical FOV.
	mCamera->setFrustumOffset(Xoffset, Yoffset);
}

void BubbleApplication::createScene(void)
{
	{//1. Create some light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);
	}
	{//2. Configure a background to adjust the projector...
	Ogre::SceneNode* wall=mSceneMgr->getRootSceneNode()->createChildSceneNode("wall", Ogre::Vector3(centerX,centerY,-Zdistance));
	//BaseApplication::createAxis(wall, screenWidth, 5);//Axis should be 5 cm thick on the wall
	//BaseApplication::createTablePlane(screenWidth, screenHeight,0,wall,"Examples/Chess");
		//2.b) Cube to the left
		{
			Ogre::SceneNode* aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("cube.mesh");
			auxEnt->setMaterialName("Examples/OgreLogo");
			aux->attachObject(auxEnt);
			aux->scale(0.1,0.1,0.1);	
			aux->setPosition(screenLeft,0,0);
		}
		//2.c) Cube to the right
		{
			Ogre::SceneNode* aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("cube.mesh");
			auxEnt->setMaterialName("Examples/OgreLogo");
			aux->attachObject(auxEnt);
			aux->scale(0.1,0.1,0.1);		
			aux->setPosition(screenRight,0,0);
		}
		//2.d) Cube to the top
		{
			Ogre::SceneNode* aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("cube.mesh");
			auxEnt->setMaterialName("Examples/OgreLogo");
			aux->attachObject(auxEnt);
			aux->scale(0.1,0.1,0.1);	
			aux->setPosition(0,screenTop,0);
		}
		//2.e) Cube to the bottom
		{
			Ogre::SceneNode* aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("cube.mesh");
			auxEnt->setMaterialName("Examples/OgreLogo");
			aux->attachObject(auxEnt);
			aux->scale(0.1,0.1,0.1);		
			aux->setPosition(0,screenBottom,0);
		}
	}
	//2. Load a sphere
	Primitives::createSphere("sampleBubble.mesh", 1, 8,8);
}


//This method is called every frame.
bool BubbleApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//myTime+=evt.timeSinceLastFrame;
	//printf("%f \n", myTime);
	//1. Mark all our nodes as dirty... if they are not updated, they will be removed
	setAllNodesDirty();
	//2. Traverse bubbles, updating the position of our nodes, creating new nodes, etc...
	updateNodesPositions();
	//3. Remove dirty nodes (they poped or wathever. This is none of our business)
	removeDirtyNodes();


	//Apply Frustum related stuff
	static Ogre::Radian FOVy=2*Ogre::Math::ATan(screenHeight/(2*Zdistance));
	static Ogre::Radian FOVx=2*Ogre::Math::ATan(screenWidth/(2*Zdistance));
    static float Xoffset=centerX/Zdistance;
	static float Yoffset=centerY/Zdistance;
	static float x=0,y=0,z=0;
    mCamera->setFOVy(FOVy);//60 degrees vertical FOV.
	mCamera->setFrustumOffset(Xoffset, Yoffset);
	mCamera->setPosition(x,y,z);
	static const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
	//printf("FPS: %f\n", stats.avgFPS);
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
		if(it->second.dirty){
			mSceneMgr->getRootSceneNode()->removeChild(it->second.node);
			it=graphicBubbles.erase(it);			
		}
		else it++;
}
int init=1;

void BubbleApplication::updateNodesPositions(){
	static Ogre::SceneNode* aux=NULL;
	
	//0. Get current position of bubbles
	static BubbleState& bubbles=BubbleState::instance();
	std::map<unsigned int, Bubble>* curBubbles= bubbles.getCurrentState();
	std::map<unsigned int, Bubble>::iterator itBubbles=curBubbles->begin();
	//1. Traverse each bubble
	for(;itBubbles!=curBubbles->end();itBubbles++){

		//--------------------------------------Init filter----------------------------
		//if(curBubbles->find(itBubbles->first)==curBubbles->end()){
		//	initFilter(itBubbles->first);
		//}
		////-----------------------------------------------------------------------------
		//if(itBubbles->second.updated){
		//Point3f center = itBubbles->second.read();
		//assignFilterVals((int)center.x, (int)center.y);
		//if(init>2)
		//		//Correct once in five measures.
		//		correctFilter(itBubbles->first);
		//init++;
		//	}
		////-----------------------------------------------------------------------------
		////Using second order model
		//	predict(itBubbles->first, myTime); 

		//1.1. Try and get a handler of the graphical representation of this bubble
		std::map<unsigned int, _GraphicalBubble>::iterator it=graphicBubbles.find(itBubbles->first);
		//1.2. Check if it already existed...
		if(it!=graphicBubbles.end())
		{//This bubble already existed... we simply update its position
			static float size=50;
			it->second.node->setScale(size,-size,size);	
			//it->second.node->setPosition(predictor[itBubbles->first].prediction.x,predictor[itBubbles->first].prediction.y, itBubbles->second.center.z); 
			//printf("%f %f\n", predictor[itBubbles->first].prediction.x,predictor[itBubbles->first].prediction.y);
			it->second.dirty=false;
		}
		else{

			//We need to set up a whole new bubble
			aux=mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* auxEnt=mSceneMgr->createEntity("sampleBubble.mesh");
			auxEnt->setMaterialName(itBubbles->second.material);//itBubbles->second.materialToShow
			aux->attachObject(auxEnt);
			//Set its size and position
			//aux->scale(itBubbles->second.radius,itBubbles->second.radius,-itBubbles->second.radius);	
			aux->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(3.1415f));
			aux->scale(40,-40,40);	
			//aux->setPosition(predictor[itBubbles->first].prediction.x,predictor[itBubbles->first].prediction.y, itBubbles->second.center.z);
			//Store it
			_GraphicalBubble gb;
			gb.dirty=false;
			gb.node=aux;
			graphicBubbles[itBubbles->first]=gb;	
		}
	}
	delete curBubbles;
	/*
	//Let's see what happens when fliping normals
	static int i=0;
	i=(i+1)%20;
	if(i==0)
		aux->scale(1,1,-1);	*/

}

