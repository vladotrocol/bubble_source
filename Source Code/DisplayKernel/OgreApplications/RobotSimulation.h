#ifndef _ROBOT_SCENE_MANAGER_H
#define _ROBOT_SCENE_MANAGER_H
#include "./DisplayKernel/OgreApplications/SimulationManager.h"



class RobotSimulationManager: public SimulationManager{
Ogre::Entity *e;
Ogre::AnimationState* mAnimationState;
public:
	std::vector<Ogre::AnimationState*> animations;
	RobotSimulationManager(){;}
	void createScene(Ogre::SceneManager* scene, Ogre::SceneNode* rootNode){
		//This is the very first thing you must do. Moreover if you are planning to use createAxis or createTablePlane
		this->rootNode=rootNode;
		mSceneMgr=scene;
		createTablePlane(5.1,2.8,-1.0, rootNode );//Size and depth of the mask panel
		//rootNode->scale(0.2f,0.2f,0.2f);
		createAxis();
		int even=0;		
		for(float i=-2.4f;i<2.5f;i+=0.55f)
			for(float j=-1.2f;j<1.5f;j+=0.55f){
				e=scene->createEntity("robot.mesh");
				if(++even%2)
					mAnimationState = e->getAnimationState("Idle");
				else 
					mAnimationState = e->getAnimationState("Walk");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
				animations.push_back(mAnimationState);
				Ogre::SceneNode* robotNode=rootNode->createChildSceneNode();
				Ogre::SceneNode* fixedNode=robotNode->createChildSceneNode();
				fixedNode->scale(0.012f,0.012f,0.014f);
				fixedNode->pitch(Ogre::Radian(1.5708f));
				fixedNode->attachObject(e);
				robotNode->setPosition(i,j,-1.2f);
			}	
		
	}
	void update(float timeSinceLastFrame){
		for(size_t i=0;i<animations.size();i++)
			switch(i%3){
				case 0:
					animations[i]->addTime(timeSinceLastFrame);
					break;
				case 1:
					animations[i]->addTime(1.25f*timeSinceLastFrame);
					break;
				case 2:
					animations[i]->addTime(0.8f*timeSinceLastFrame);
					break;

			}	
	}

	virtual void  keyPressed( const OIS::KeyEvent &arg ){

		if( arg.key == OIS::KC_F1)   
		{
			//Translate +0.1 in the Z axis.
			rootNode->translate(0,0,0.1);
		}
		if(arg.key == OIS::KC_F2)   
		{
			//Translate -0.1 in the Z axis.
			rootNode->translate(0,0,-0.1);
		}

	}


	virtual void mouseMoved( const OIS::MouseEvent &arg ){
		//arg.state.X.rel ->Horizonjtal movement
		//arg.state.Y.rel -> Vertical movement
		//arg.state.Z.rel -> Wheel movement
		if(arg.state.Z.rel==0)
			return ;
		if(arg.state.Z.rel>0)
			rootNode->translate(0,0,0.1);
		else 
			rootNode->translate(0,0,-0.1);
	}

};
#endif