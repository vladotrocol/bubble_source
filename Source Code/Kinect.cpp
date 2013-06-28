#include "Kinect.h"

//Constructor
Kinect::Kinect(void){
};

//-------------------------API------------------------------

//Initialise Kinect
bool Kinect::initialiseKinect(void){
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,2,NULL,&depthStream);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &rgbStream); 
	return sensor;
};

//Get the next frame
bool Kinect::hasNextFrame(char s, NUI_IMAGE_FRAME *imageFrame){
	HANDLE stream = whichStream(s);
	if(stream){
		if(sensor->NuiImageStreamGetNextFrame(stream, 10, imageFrame) >= 0){
			return true;
		}
	}
	//cerr << "No frame available\n";
	return false;
};

//Releases the current frame
bool Kinect::releaseFrame(char s, NUI_IMAGE_FRAME *imageFrame){
	return sensor->NuiImageStreamReleaseFrame(whichStream(s), imageFrame);
};

//Compute the correct depth image data in milimiters
BYTE* Kinect::getDepthData(NUI_LOCKED_RECT *LockedRect){
	BYTE data[width*height*3];
	int j = 0;
	const USHORT* curr = (const USHORT*) LockedRect->pBits;
	const USHORT* dataEnd = curr + (width*height);
	while (curr < dataEnd) {
		USHORT depth = NuiDepthPixelToDepth(*curr++);
		for (int i = 0; i < 3; ++i){
			data[j] = (BYTE) depth%256;
		}
		j++;
	}
	return data;
};

//------------------------------Internal-------------------------

//Returns the requested stream
HANDLE Kinect::whichStream(char s){
	if(s == 'r'){
		return rgbStream;
	}
	else if(s == 'd'){
		return depthStream;
	}
	else{
		cerr<<"Flag 1 is incorrect";
		return NULL;
	}
};