#ifndef _BUBBLE_APPLICATION
#define _BUBBLE_APPLICATION
#include <./DisplayKernel/OgreApplications/BaseApplication.h>
//#include <./DisplayKernel/OgreUtils/CameraOISController.h>

class _GraphicalBubble{
public:	
	bool dirty;
	Ogre::SceneNode* node;
};

class BubbleApplication:public BaseApplication{
	//SceneNodeController* camControl;
	//These are our Ogre nodes (inverted spheres of the appropriate size and with the appropriate material applied...)
	std::map<unsigned int, _GraphicalBubble> graphicBubbles;
public:
	  virtual void createCamera(void);
	  virtual void createScene(void);
	  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	  void setAllNodesDirty();
	  void updateNodesPositions();
	  void removeDirtyNodes();


};


#endif