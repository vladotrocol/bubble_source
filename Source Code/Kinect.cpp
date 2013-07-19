#include "Kinect.h"

unsigned int static const width=640; //width of kinect stream
unsigned int static const height=480; //height of kiect stream


//Constructor
Kinect::Kinect(void)
{;};

//-------------------------Public------------------------------

//Initialise Kinect
void Kinect::initialiseKinect(void){
	int numSensors;
	isInit = false;
	if (!(NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) && !(NuiCreateSensorByIndex(0, &sensor) < 0)){
		sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
		sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,2,NULL, &depthStream);
		sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &rgbStream);
		isInit = true;
	}
};

//Check if Kinect is detected
bool Kinect::hasInitialized(){
	return isInit;
};

//Get the next frame
bool Kinect::hasNextFrame(char s, NUI_IMAGE_FRAME *imageFrame){
	stream = whichStream(s);
	if(*stream&&isInit){
		if(sensor->NuiImageStreamGetNextFrame(*stream, 10, imageFrame) >= 0){
			return true;
		}
	}
	return false;
};

//Releases the current frame
HRESULT Kinect::releaseFrame(char s, NUI_IMAGE_FRAME *imageFrame){
	return sensor->NuiImageStreamReleaseFrame(*whichStream(s), imageFrame);
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

//------------------------------Private-------------------------

//Returns the requested stream
HANDLE* Kinect::whichStream(char s){
	if(s == 'r'){
		return &rgbStream;
	}
	else if(s == 'd'){
		return &depthStream;
	}
	else{
		cerr<<"Flag 1 is incorrect";
		return &depthStream;
	}
};