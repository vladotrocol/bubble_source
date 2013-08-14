#ifndef _KINECT_C
#define _KINECT_C

#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <string>

#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <nuisensor.h>


using namespace std;
using namespace cv;
class Kinect{
	public:
		USHORT* dataMil;
		BYTE* dataPix;
		HANDLE rgbStream; //kinect colour stream
		HANDLE depthStream; //kinect depth stream
		Kinect(void); //Constructor
		void initialiseKinect(void); //Initialise Kinect
		bool hasNextFrame(NUI_IMAGE_FRAME *imageFrame); //Get the next frame
		HRESULT releaseFrame(NUI_IMAGE_FRAME *imageFrame); //Releases the current frame
		void getDepthData(NUI_LOCKED_RECT *LockedRect); //Compute the correct depth image data in milimiters
		bool hasInitialized(void); //Check if Kinect is detected

	private:
		INuiSensor* sensor; //Pointer to kinect sensors
		HANDLE* whichStream(char s); //Returns the requested stream
		HANDLE* stream; //Main kinect stream
		bool isInit; //Kinect initialised
};

#endif