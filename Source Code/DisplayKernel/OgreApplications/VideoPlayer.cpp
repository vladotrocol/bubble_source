#include "./DisplayKernel/OgreApplications/VideoPlayer.h"
#include "./DisplayKernel/OgreUtils/CameraManager.h"
#include "./DisplayKernel/OgreUtils/ViewportData.h"
#include "./DisplayKernel/OgreUtils/CameraData.h"
#include "./DisplayKernel/OgreUtils/CameraUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


//-------------------------------------------------------------------------------------
VideoPlayerApplication::VideoPlayerApplication(std::string videoFile, std::string out_TextureName="videoTexture")
	: videoFile(videoFile)
	, out_TextureName(out_TextureName)
	
{
}
//-------------------------------------------------------------------------------------
VideoPlayerApplication::~VideoPlayerApplication(void)
{
	//txPtr->unload();
}
//-------------------------------------------------------------------------------------

void VideoPlayerApplication::chooseSceneManager(void)
{

    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	


}
int VideoPlayerApplication::createViewports(int curIndex)
{
	// Create one viewport, entire window
    return ++curIndex;
}

void VideoPlayerApplication::createCamera(void)
{
;
}

void VideoPlayerApplication::createScene(void)
{
	cv::VideoCapture* p_cap=new cv::VideoCapture();
	cap=p_cap;
	p_cap->open(videoFile);
	cv::Mat image;
	(*p_cap)>>image;
	txPtr = Ogre::TextureManager::getSingleton().createManual(out_TextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TextureType::TEX_TYPE_2D,image.cols, image.rows,  0, Ogre::PixelFormat::PF_BYTE_BGR);
	txPtr->getBuffer()->blitFromMemory(Ogre::PixelBox(image.cols, image.rows,1,Ogre::PixelFormat::PF_BYTE_BGR,image.data));
}


bool VideoPlayerApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	cv::VideoCapture* p_cap=(cv::VideoCapture*)cap;
	cv::Mat image;
	(*p_cap)>>image;
	if(!image.data){
		//We reached the end of the video.
		p_cap->release();
		p_cap->open(videoFile);
		(*p_cap)>>image;
	}
	txPtr->getBuffer()->blitFromMemory(Ogre::PixelBox(image.cols, image.rows,1,Ogre::PixelFormat::PF_BYTE_BGR,image.data));
	return true;	
}
