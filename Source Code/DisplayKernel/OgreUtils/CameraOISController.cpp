#include "./DisplayKernel/OgreUtils/CameraOISController.h"

using namespace OgreUtils;

static SceneNodeController* uniqueInstance=NULL;

SceneNodeController* SceneNodeController::instance(){
		if(!uniqueInstance)
			uniqueInstance=new SceneNodeController();
		return uniqueInstance;
	}