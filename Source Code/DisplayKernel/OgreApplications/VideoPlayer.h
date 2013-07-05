#ifndef _VIDEO_PLAYER
#define _VIDEO_PLAYER
 
#include "./DisplayKernel/OgreApplications/BaseApplication.h"

class VideoPlayerApplication: public BaseApplication, Ogre::RenderTargetListener
{
public:
    VideoPlayerApplication(std::string videoFile, std::string out_TextureName);
    virtual ~VideoPlayerApplication(void);
 
protected:
    virtual void createScene(void);
	virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual int createViewports(int curIndex=0);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	float x_offset, viewportWidth;
	std::string videoFile;//NAme of the material that we are going to render at full screen
	void* cap;
	Ogre::TexturePtr txPtr; 
	std::string out_TextureName;//We render that material (CG program, Video, etc.) to a regular texture. This is the name of that texture
	
};
 
#endif // #ifndef __MatrixRTTApplication_h_