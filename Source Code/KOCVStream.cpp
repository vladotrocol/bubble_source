#include "KOCVStream.h"

//Constructor
KOCVStream::KOCVStream(Kinect* k, Filters* f):
	kinect(k), 
	filter(f)
{;};

//----------------------------API------------------------

//Display streams in windows
//FLAG grammar is: ((r||d)(t||e||i)*)*
void KOCVStream::display(char* s){
	string b = "_window_";
	generateControls();
	generateWindows(s);
	unsigned int j=0;
	while(j<strlen(s)){
		if(s[j]=='r'||s[j]=='d'){
			readFrame(s[j]);
		}
		else{
			cerr<<"Some flag error in display function";
			break;
		}
		unsigned int i=j;
		do{
			imshow(s[j]+b+s[i], filter->applyFilter(s[i],*whichSource(s[j])));
			waitKey( 20 );
			i++;
		}while((char)s[i]!='r'&&(char)s[i]!='d'&&i<strlen(s));
		j=i;
	}	
	char c = waitKey( 20 );
};

//Reads the current frames into the source materials
void KOCVStream::readFrame(char s){
	Mat temp;
	while (temp.empty())
		temp = tryReadFrame(s);
	if(s=='r'){
		temp.copyTo(rgb_src);
	}
	else if(s=='d'){
		temp.copyTo(depth_src);
	}
	temp.release();
};

//Display detected bubbles as circles
void KOCVStream::displayBubbles(vector<Bubble> &bubbles){
	RNG rng(12345);
	Mat drawing = Mat::zeros(depth_src.size(), CV_8UC3 );
	for( unsigned int i = 0; i< bubbles.size(); i++ ){
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		if((int)bubbles[i].radius>0)
        circle( drawing, bubbles[i].center, (int)bubbles[i].radius, color, 2, 8, 0 );
    }
    imshow( "Contours", drawing );
    cvWaitKey(10);
};

//--------------------------Internal----------------------------

//Read the Mat data into a image frame
Mat KOCVStream::tryReadFrame(char s){
	NUI_IMAGE_FRAME imageFrame;

	if (!kinect->hasNextFrame(s, &imageFrame)) {
		return Mat();
	}

	Mat depthFrame = kFrameToMat(s, imageFrame);
	kinect->releaseFrame(s, &imageFrame);

	if(s == 'd'){
		flip(depthFrame, depthFrame, 1);
	}

	return depthFrame;
};

//Put the kinect frame data onto a Mat
Mat KOCVStream::kFrameToMat(char s, NUI_IMAGE_FRAME imageFrame){
	int dataType = whichDataType(s);
	Mat frame(height, width, dataType);
	NUI_LOCKED_RECT LockedRect;

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	BYTE* imageData;
	imageData = whichImageData(s,&LockedRect);
	if (LockedRect.Pitch != 0 && imageData){
		Mat tempMat(height, width, dataType, imageData);
		tempMat.copyTo(frame);
	}
	else{
		return Mat();
	}

	texture->UnlockRect(0);

	if(dataType == CV_8UC4){
		frame.convertTo(frame, CV_8UC3);
	}

	return frame;
};

//Get the image data correspondig to flag
BYTE* KOCVStream::whichImageData(char s, NUI_LOCKED_RECT *LockedRect){
	BYTE* data;
	if(s == 'r'){
		data = (*LockedRect).pBits;
	}
	else if(s == 'd'){
		data = kinect->getDepthData(LockedRect);
	}
	else{
		cerr<<"Flag 1 is incorrect";
		data =  NULL;
	}
	return data;
};

//This function returns the correct data format for each stream type
int KOCVStream::whichDataType(char s){
	if(s=='r')
		return CV_8UC4;
	else if(s == 'd'){
		return CV_8U;
	}
	else{
		cerr<<"invalid datatype";
		return -1;
	}
};

//Returns the correct source
Mat* KOCVStream::whichSource(char s){
	if(s == 'r'){
		return &rgb_src;
	}
	else if(s == 'd'){
		return &depth_src;
	}
	else{
		cerr<<"Incorrect source";
		return NULL;
	}
};


//Generates windows according to grammar
void KOCVStream::generateWindows(char* s){
string b = "_window_";
	unsigned int j=0;
	while(j<strlen(s)){
			if(s[j]=='r'||s[j]=='d'){
				readFrame(s[j]);
			}
			else{
				cerr<<"Some flag error in display function";
				break;
			}
			unsigned int i=j;
			do{
				namedWindow(s[j]+b+s[i],0);
				i++;
			}while((char)s[i]!='r'&&(char)s[i]!='d'&&i<strlen(s));
			j=i;
		}	
};


//Generates the window with the filter sliders
void KOCVStream::generateControls(){
	namedWindow("Controls", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar( "ThreValue", "Controls",
                  &filter->thresholdValue, 255,
                  NULL);
	cvCreateTrackbar("ThreType","Controls",
				  &filter->thresholdType,
                  4, NULL);
	cvCreateTrackbar("EroSize","Controls",
				  &filter->erosionSize,
                  200, NULL);
	cvCreateTrackbar("DilSize","Controls",
				  &filter->dilationSize,
                  200, NULL);
};