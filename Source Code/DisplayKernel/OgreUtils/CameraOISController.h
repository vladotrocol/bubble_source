#ifndef _CAMERA_CONTROLLER_H
#define _CAMERA_CONTROLLER_H
#include "Ogre.h"
#include <limits>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <vector>

namespace OgreUtils{

class SceneNodeController{
	std::vector<Ogre::SceneNode*> mTargets;
	Ogre::Real mTopSpeed;
	Ogre::Vector3 mVelocity;
	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	bool mGoingUp;
	bool mGoingDown;
	bool mFastMove;
	bool hasTarget;

	SceneNodeController()
	: mTopSpeed(150)
	, mVelocity(Ogre::Vector3::ZERO)
	, mGoingForward(false)
	, mGoingBack(false)
	, mGoingLeft(false)
	, mGoingRight(false)
	, mGoingUp(false)
	, mGoingDown(false)
	, mFastMove(false)
	,hasTarget(false)
	{
			;
	}
public:

	static SceneNodeController* instance();
	
	void setTarget(Ogre::SceneNode* node){
		mTargets.push_back(node);
		hasTarget=true;
	}
	
	virtual void injectKeyDown(const OIS::KeyEvent& evt){
		if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = true;
		else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = true;
		else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = true;
		else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = true;
		else if (evt.key == OIS::KC_PGUP) mGoingUp = true;
		else if (evt.key == OIS::KC_PGDOWN) mGoingDown = true;
		else if (evt.key == OIS::KC_LSHIFT) mFastMove = true;
	}

	virtual void injectKeyUp(const OIS::KeyEvent& evt){
		if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = false;
		else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = false;
		else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = false;
		else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = false;
		else if (evt.key == OIS::KC_PGUP) mGoingUp = false;
		else if (evt.key == OIS::KC_PGDOWN) mGoingDown = false;
		else if (evt.key == OIS::KC_LSHIFT) mFastMove = false;
	}


	virtual void injectMouseMove(const OIS::MouseEvent& evt){
		if(!hasTarget)return;
		for(size_t i=0;i<mTargets.size();i++){
			mTargets[i]->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f),Ogre::Node::TS_WORLD);
			mTargets[i]->pitch(Ogre::Degree(evt.state.Y.rel * 0.15f),Ogre::Node::TS_LOCAL);
		}
	}

	virtual void injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id){;}

	virtual void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id){;}

	virtual void updateNodePosition(float timeSinceLastEvent){
		if(!hasTarget)return;
		Ogre::Vector4 accel(Ogre::Vector4::ZERO);
		if (mGoingLeft)	   accel[0] += timeSinceLastEvent;
		if (mGoingRight)   accel[0] -= timeSinceLastEvent;
		if (mGoingUp)	   accel[1] += timeSinceLastEvent;
		if (mGoingDown)    accel[1] -= timeSinceLastEvent;
		if (mGoingForward) accel[2] += timeSinceLastEvent;
		if (mGoingBack)	   accel[2] -= timeSinceLastEvent;
		static const float v=0.3f;
		accel*=v;
		for(size_t i=0;i<mTargets.size();i++){
			mTargets[i]->translate(Ogre::Vector3(accel[0],accel[1],accel[2]),Ogre::Node::TS_LOCAL);
		}
		
		

		

	
	}

};

}
#endif