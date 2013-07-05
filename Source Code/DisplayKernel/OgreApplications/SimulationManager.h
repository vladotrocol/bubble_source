#ifndef _SIMULATION_MANAGER_H
#define _SIMULATION_MANAGER_H
#include "Ogre.h"
#include "OIS.h"


class SimulationManager{

public:
	virtual void createScene(Ogre::SceneManager* scene, Ogre::SceneNode* rootNode)=0;
	virtual void update(float timeSinceLastFrame)=0;
	//Keyboard
	virtual void keyPressed( const OIS::KeyEvent &arg ){;}
    virtual void keyReleased( const OIS::KeyEvent &arg ){;}
    // Mouse
    virtual void mouseMoved( const OIS::MouseEvent &arg ){;}
    virtual void mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){;}
    virtual void mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){;}
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* rootNode;
	void createAxis(Ogre::SceneNode* r_node=NULL){
		if(!r_node)
			r_node=mSceneMgr->getRootSceneNode();
		static const float SCALE=0.01f;
		Ogre::Entity* model = mSceneMgr->createEntity("axis_X", "cube.mesh");
		Ogre::SceneNode* node = r_node->createChildSceneNode();
		node->scale(100*SCALE,SCALE*0.01f,SCALE*0.01f);
		model->setMaterialName("Examples/Red");
		node->attachObject(model);

		model = mSceneMgr->createEntity("axis_Y", "cube.mesh");
		node = r_node->createChildSceneNode();
		node->scale(SCALE*0.01f,SCALE*100.01f,SCALE*0.01f);
		model->setMaterialName("Examples/Green");
		node->attachObject(model);

		model = mSceneMgr->createEntity("axis_Z", "cube.mesh");
		node =r_node->createChildSceneNode();
		node->scale(SCALE*0.01f,SCALE*0.01f,SCALE*100.01f);
		model->setMaterialName("Examples/Blue");
		node->attachObject(model);

		{
		model = mSceneMgr->createEntity("leftTag", "cube.mesh");
		node =r_node->createChildSceneNode();
		node->scale(SCALE*0.01f,SCALE*100.01f,SCALE*0.01f);
		model->setMaterialName("Examples/Blue");
		node->attachObject(model);
		node->translate(-1,0,0);

		model = mSceneMgr->createEntity("rightTag", "cube.mesh");
		node = r_node->createChildSceneNode();
		node->scale(SCALE*0.01f,SCALE*100.01f,SCALE*0.01f);
		model->setMaterialName("Examples/Blue");
		node->attachObject(model);
		node->translate(1,0,0);
		}
}

	void createTablePlane(float sizex, float sizey, float offsetz, Ogre::SceneNode* node=NULL, std::string materialName="Examples/Water2"){
		if(!node)
			node=this->mSceneMgr->getRootSceneNode();

		Ogre::MovablePlane* mPlane;
		Ogre::Entity*       mPlaneEnt;
		Ogre::SceneNode*    mPlaneNode;
		mPlane = new Ogre::MovablePlane("Table");
		mPlane->d = 0;
		mPlane->normal = Ogre::Vector3::UNIT_Z;
		Ogre::MeshManager::getSingleton().createPlane("TableMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, sizex, sizey, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
		mPlaneEnt = mSceneMgr->createEntity("TableEntity", "TableMesh");
		//1.b) Apply the material to the plane
		mPlaneEnt->setMaterialName(materialName);
		//2. The Plane node with the Plane entity attached to it
		mPlaneNode = node->createChildSceneNode("_TableNode");
		mPlaneNode->translate(0,0,offsetz);
		mPlaneNode->attachObject(mPlaneEnt);


}
};
#endif