#ifndef _VIEWPORT_DATA
#define _VIEWPORT_DATA
#include <Ogre.h>
#include <string>

namespace OgreUtils{

class ViewportData{
	
	std::string viewportName;
	std::string cameraName;
	Ogre::Camera* camera;
	float x_offset;
	float y_offset;
	float width;
	float height;
	int z_order;
	Ogre::Viewport* viewport;
	bool bound;
	bool overlays;
public:
	ViewportData(std::string viewportName,float x_offset,float y_offset,float width,float height,std::string cameraName, int z_order)
	: viewportName(viewportName)
	, cameraName(cameraName)
	, x_offset(x_offset)
	, y_offset(y_offset)
	, width(width)
	, height(height)
	, z_order(z_order)
	, viewport(NULL)
	, camera(NULL)
	, bound(false)
	, overlays(z_order==0)//Overlays are shown ONLY on the first viewport
	{;}

	void reset(){
		viewport=NULL;
		bound=false;
	}

	inline void bind(Ogre::Camera* cam){
		bind(cam, Ogre::Root::getSingleton().getAutoCreatedWindow());
		this->camera=cam;
		//cameraName=cam->getName(); //CHECK THIS!!!
	}

	inline void bind(Ogre::Camera* cam, Ogre::RenderWindow* w){
		if(viewport==NULL)
			viewport=w->addViewport(cam,z_order,x_offset,y_offset,width,height);
		else
			viewport->setCamera(cam);
		viewport->setOverlaysEnabled(this->overlays);
		bound=true;	
	}

	inline const std::string& getViewportName(){
		return viewportName;	
	}

	inline Ogre::Camera* getCamera(){
		return camera;	
	}
	inline std::string getCameraName(){
		return cameraName;	
	}
	inline bool isBound(){
		return bound;	
	}
};

}
#endif
