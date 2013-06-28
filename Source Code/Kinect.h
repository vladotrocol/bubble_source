#ifndef _KINECT_C
#define _KINECT_C

#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <string>

using namespace std;

#define width 640 
#define height 480

class Kinect{
	public:
		HANDLE rgbStream;
		HANDLE depthStream;
		INuiSensor* sensor;

		//External API
		Kinect(void);
		bool initialiseKinect(void);
		bool hasNextFrame(char s, NUI_IMAGE_FRAME *imageFrame);
		bool releaseFrame(char s, NUI_IMAGE_FRAME *imageFrame);
		BYTE* getDepthData(NUI_LOCKED_RECT *LockedRect);

	private:
		//Internal
		HANDLE whichStream(char s);
};

#endif