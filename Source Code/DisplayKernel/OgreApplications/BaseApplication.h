
#ifndef __BaseApplication_h_
#define __BaseApplication_h_
#include <./DisplayKernel/OgreApplications/IOgreApplication.h>

class BaseApplication : public IOgreApplication, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    virtual void go(void);
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual int createViewports(int curIndex=0);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
protected: 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	// WindowEventListener
	//Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);



   
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    // OgreBites
    bool mShutDown;

public:
	void createAxis(Ogre::SceneNode* r_node=NULL, float length=100, float width=0.01f){
		if(!r_node)
			r_node=mSceneMgr->getRootSceneNode();
		static const float SCALE=0.01f;
		Ogre::Entity* model = mSceneMgr->createEntity("axis_X", "cube.mesh");
		Ogre::SceneNode* node = r_node->createChildSceneNode();
		node->scale(length*SCALE,SCALE*width,SCALE*width);
		node->translate(length/2,0,0);
		model->setMaterialName("Examples/Red");
		node->attachObject(model);

		model = mSceneMgr->createEntity("axis_Y", "cube.mesh");
		node = r_node->createChildSceneNode();
		node->scale(SCALE*width,SCALE*length,SCALE*width);
		node->translate(0,length/2,0);
		model->setMaterialName("Examples/Green");
		node->attachObject(model);

		model = mSceneMgr->createEntity("axis_Z", "cube.mesh");
		node =r_node->createChildSceneNode();
		node->scale(SCALE*width,SCALE*width,SCALE*length);
		node->translate(0,0,length/2);
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

#endif // #ifndef __BaseApplication_h_
