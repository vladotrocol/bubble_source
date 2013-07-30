#include "KOCVStream.h"
CvSize size = cvSize(640,480); //Size for recording video

//Constructor
KOCVStream::KOCVStream(){
	kinect = new Kinect();
	filter = new Filters();
	kinect->initialiseKinect();
};

//Reads the current frames into the source materials
void KOCVStream::readFrame(){
	Mat* temp = new Mat();
	//We only ask for the next non-empty kinect frame
	while (temp->empty()){
		temp = tryReadFrame();
	}

	//Assign the frame to main stream
	_stream = temp;
};

//--------------------------Internal----------------------------

NUI_IMAGE_FRAME* imageFrame = new NUI_IMAGE_FRAME(); //from kinect stream
Mat* depthFrame = new Mat();

//Get the Mat frame
Mat* KOCVStream::tryReadFrame(){

	//Proceed only if kinnect has a frame
	if (!kinect->hasNextFrame(imageFrame)) {
		return new Mat();
	}

	//Convert the kinect frame to a Mat
	depthFrame = kFrameToMat(imageFrame);
	kinect->releaseFrame(imageFrame);

	//Depth frame need to be flipped
	flip(*depthFrame, *depthFrame, 1);//or not? IMPROVE:
	
	return depthFrame;
};

//Put the kinect imageframe data onto a Mat
Mat* KOCVStream::kFrameToMat(NUI_IMAGE_FRAME* imageFrame){

	Mat* frame = new Mat(height, width, CV_8U);
	NUI_LOCKED_RECT LockedRect;

	//Lock the imageFrame such that kinnect cannot write on it
	INuiFrameTexture* texture = imageFrame->pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	//Get the kinect depth data
	BYTE* imageData = new BYTE();
	imageData =  kinect->getDepthData(&LockedRect);

	//If the data is not empty convert it to Mat
	if (LockedRect.Pitch != 0){
		Mat tempMat(height, width, CV_8U, imageData);
		tempMat.copyTo(*frame);
	}
	else{
		return new Mat();
	}
	
	//Release the frame
	texture->UnlockRect(0);

	return frame;
};

//Record kinect data into a file
void KOCVStream::record(char* fileName){
	
	Mat* image = new Mat(); //frame to be written to file

	//We need a window for the keylisteners
	namedWindow("record panel",0);

	//Set up the writer
	VideoWriter writer(fileName,-1,30,size);
	if (!writer.isOpened()){
        std::cout << "Output video could not be opened" << std::endl;
        return;
	}

	//Don't start until 'r' is pressed
	cout<<"press r to record\n";
	while(1){
	char x = waitKey(10);
	if( (char)x == 'r' ){
			break; 
		}
	}

	//Recording
	cout<<"Recording...\nPress s to stop.\n";
	while(1){
		
		//When s is pressed stop
		char x = waitKey(10);
		if((char)x=='s'){
			break;
		}

		//Read the image
		image = tryReadFrame();
		
		//Optionally display it on the screen
		imshow("record panel",*image);
		waitKey( 20 );

		//Write it to file
		writer.write(*image);
	}

	cout<<"finished recording";
};
