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
	dataMil = new USHORT[width*height];
	dataPix = new BYTE[width*height];
	if (!(NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) && !(NuiCreateSensorByIndex(0, &sensor) < 0)){
		sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
		sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480,0 ,2,NULL, &depthStream); //0x00040000 for white background //NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE
		sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &rgbStream);
		isInit = true;
	}
};

//Check if Kinect is detected
bool Kinect::hasInitialized(){
	return isInit;
};

//Get the next frame
bool Kinect::hasNextFrame(NUI_IMAGE_FRAME *imageFrame){
	stream = &depthStream;
	if(*stream&&isInit){
		if(sensor->NuiImageStreamGetNextFrame(*stream, 10, imageFrame) >= 0){
			return true;
		}
	}
	return false;
};

//Releases the current frame
HRESULT Kinect::releaseFrame(NUI_IMAGE_FRAME *imageFrame){
	return sensor->NuiImageStreamReleaseFrame(depthStream, imageFrame);
};


//Compute the correct depth image data in milimiters
void Kinect::getDepthData(NUI_LOCKED_RECT *LockedRect){

	int j = 0;

	const USHORT* curr = (const USHORT*) LockedRect->pBits;
	const USHORT* dataEnd = curr + (width*height);
	
	
	static float max = 0;
	if (LockedRect->Pitch != 0){
	while (curr < dataEnd) {
		USHORT depth = NuiDepthPixelToDepth(*curr++);
		dataMil[j] = depth;
		if(depth>max){
			max = depth;
		}
		j++;
	}

	for(int i=0;i<width*height;i++){
		dataPix[i] = (BYTE)(dataMil[i]/max*255);
		if(dataPix[i]!=0){
			dataPix[i] = 255- dataPix[i];
			//dataMil[i] = 16*dataMil[i];//Expand depth image (0,4000) to ful 16 bit range
		}
	}
	}
};

//Vector4 data = NuiTransformDepthImageToSkeletonF();