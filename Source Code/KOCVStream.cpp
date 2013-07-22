#include "KOCVStream.h"

CvSize size = cvSize(640,480); //Size for recording video

//Constructor
KOCVStream::KOCVStream(){
	TestON = false;
	kinect = new Kinect();
	filter = new Filters();
	kinect->initialiseKinect();
};

//Reads the current frames into the source materials
void KOCVStream::readFrame(){
	Mat* temp = new Mat();
	while (temp->empty())
		*temp = tryReadFrame();
		_stream = temp;
};

//--------------------------Internal----------------------------

//Read the Mat data into a image frame
Mat KOCVStream::tryReadFrame(){
	NUI_IMAGE_FRAME imageFrame;

	if (!kinect->hasNextFrame(&imageFrame)) {
		return Mat();
	}

	Mat depthFrame = kFrameToMat(imageFrame);
	kinect->releaseFrame(&imageFrame);

	flip(depthFrame, depthFrame, 1);

	return depthFrame;
};

//Put the kinect frame data onto a Mat
Mat KOCVStream::kFrameToMat(NUI_IMAGE_FRAME imageFrame){
	Mat frame(height, width, CV_8U);
	NUI_LOCKED_RECT LockedRect;

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	BYTE* imageData;
	imageData =  kinect->getDepthData(&LockedRect);
	if (LockedRect.Pitch != 0 && imageData){
		Mat tempMat(height, width, CV_8U, imageData);
		tempMat.copyTo(frame);
	}
	else{
		return Mat();
	}

	texture->UnlockRect(0);

	return frame;
};

//Record kinect data into a file
void KOCVStream::record(char* fileName){
	VideoWriter writer(fileName,-1,30,size);
	if (!writer.isOpened()){
        std::cout << "!!! Output video could not be opened" << std::endl;
        return;
	}
	Mat image;
	namedWindow("record panel",0);
	cout<<"press r to record\n";
	while(1){
	char x = waitKey(10);
	if( (char)x == 'r' ){
			break; 
		}
	}
	cout<<"Recording...\nPress s to stop.\n";
	while(1){
		char x = waitKey(10);
		if((char)x=='s'){
			break;
		}
		image = tryReadFrame();
		imshow("record panel",image);
		waitKey( 20 );
		writer.write(image);
	} 
	cout<<"finished recording";
};
