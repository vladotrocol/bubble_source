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
class Kinect{
	public:
		HANDLE rgbStream; //kinect colour stream
		HANDLE depthStream; //kinect depth stream

		Kinect(void); //Constructor
		void initialiseKinect(void); //Initialise Kinect
		bool hasNextFrame(char s, NUI_IMAGE_FRAME *imageFrame); //Get the next frame
		HRESULT releaseFrame(char s, NUI_IMAGE_FRAME *imageFrame); //Releases the current frame
		BYTE* getDepthData(NUI_LOCKED_RECT *LockedRect); //Compute the correct depth image data in milimiters
		bool hasInitialized(void); //Check if Kinect is detected

	private:
		INuiSensor* sensor; //Pointer to kinect sensors
		HANDLE whichStream(char s); //Returns the requested stream
		HANDLE stream; //Main kinect stream
		bool isInit; //Kinect initialised
};

#endif