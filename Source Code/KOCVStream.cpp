#include "KOCVStream.h"
CvSize size = cvSize(640,480); //Size for recording video

//Constructor
KOCVStream::KOCVStream(){
	kinect = new Kinect();
	filter = new Filters();
	kinect->initialiseKinect();
	dataMil = kinect->dataMil;
	dataPix = kinect->dataPix;
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

void KOCVStream::displayBubbles(vector<Bubble> &bubbles){
	RNG rng(12345);
	//Mat drawing(480,640,CV_16U,kinect->dataMil);//Copy depth data
	Mat drawing=Mat::zeros(480,640,CV_8U);
	/*for (int x =0; x < 640; x++) 
		for (int y = 0; y < 480; y++) 
			drawing.at<uchar>(Point2f(x, y))=(unsigned char)(this->kinect->dataMil[(int)(x+640*y)]*256/4000);*/


	for( unsigned int i = 0; i< bubbles.size(); i++ ){
		//Copy the elements within the contour
		/*int rad=(int)bubbles[i].radius;
		int b_centre_x=(int)bubbles[i].center.x;
		int b_centre_y=(int)bubbles[i].center.y;
		for (int x = -rad; x < rad; x++) 
			for (int y = -rad; y < rad; y++) 
				if ((x*x + y*y)<(rad*rad)) {
					float aux_x=x+b_centre_x;
					float aux_y=y+b_centre_y;
					drawing.at<BYTE>(Point2f(aux_x, aux_y))=(unsigned char)(this->kinect->dataMil[(int)(aux_x+640*aux_y)]*256/4000);
				}*/
		//draw the contour
		Scalar color = Scalar( 255, 255, 255 );
		if((int)bubbles[i].radius>0)
        circle( drawing, Point2f(bubbles[i].center.x, bubbles[i].center.y), (int)bubbles[i].radius, color, 2, 8, 0 );
    }
    imshow( "Contours", drawing );
    cvWaitKey(10);
};

//Put the kinect imageframe data onto a Mat
Mat* KOCVStream::kFrameToMat(NUI_IMAGE_FRAME* imageFrame){

	Mat* frame = new Mat(height, width, CV_8U);
	
	NUI_LOCKED_RECT LockedRect;

	//Lock the imageFrame such that kinnect cannot write on it
	INuiFrameTexture* texture = imageFrame->pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	//Get the kinect depth data
	BYTE* imageData;
	
	kinect->getDepthData(&LockedRect);
	imageData = kinect->dataPix;
	
	//If the data is not empty convert it to Mat
	if (LockedRect.Pitch != 0){
		/* //Do not do new above!
		   frame=new Mat(height, width, CV_8U, imageData);	
		*/
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
