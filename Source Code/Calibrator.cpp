#include "Calibrator.h"

bool homographyComputed;
bool homogCamComputed;

Mat src, distortion, H, iH, faceImg;

int dstCount; //iterator for stream points
int srcCount; //iterator for reference points

Stream* STREAM; //Pointer to main stream

Point2f dstPoints[4]; //Stream points
Point2f srcPoints[4]; //Reference image points

//Constructor
Calibrator::Calibrator(Stream* stream):s(stream){};

//Checks if all points have been clicked in both windows, if so computes the homography
void tryComputeHomography(Point2f srcPoints[],int srcCount, Point2f dstPoints[], int dstCount,Mat source, Mat distortion){
	
	Mat result;

	//Proceed only when the 8 reference points have been set
	if(srcCount<4 || dstCount<4)
		return;

	// Calculate the perspective transform from 4 pairs of corresponding points
	H=getPerspectiveTransform(dstPoints,srcPoints);

	// Create a zeroed matrix the same size as the distortion matrix
	result = Mat::zeros(distortion.rows, distortion.cols, distortion.type());

	// Apply the perspective transformation to the 'result' matrix
	warpPerspective(distortion, result, H, result.size());

	//Set up the windows
	cvSetWindowProperty("distortion", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
	cvSetWindowProperty("source", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
	
	// Display the perspective transformation image
	imshow("distortion", result);

	homographyComputed = true;

	return;
 }


//Function triggered by the mouse listener on image source
void on_mouse_source(int evt, int x, int y, int flags, void* param){
	
	//Proceed only if left button is clicked
	if( evt != CV_EVENT_LBUTTONDOWN )
        return;

	//Or if we haven't already clicked all 4 points
	if(srcCount>=4)
		return;

	//Set one of the source points as the newly clicked point
	srcPoints[srcCount]=Point2f((float)x,(float)y);

	//Draw a black circle to mark the clicked spot
	circle(src, srcPoints[srcCount], 5, Scalar(1.0, 1.0, 0),5);
	
	srcCount++;

	//Display image
	imshow("source",src);

	//Next recursive step
	tryComputeHomography(srcPoints, srcCount, dstPoints, dstCount, src, distortion);
};

//Function triggered by the mouse listener on kinect stream
void on_mouse_distortion(int evt, int x, int y, int flags, void* param){

	//Proceed only if left button is clicked
	if( evt != CV_EVENT_LBUTTONDOWN )
		return;

	//Or if we haven't already clicked all 4 points
	if(dstCount>=4)
		return;

	//Set one of the source points as the newly clicked point
	dstPoints[dstCount]=Point2f((float)x,(float)y);
	
	//Get a new frame from stream
	distortion = *STREAM->_stream;

	//Draw a black circle to mark the clicked spot
	circle(distortion, dstPoints[dstCount], 5, Scalar(0.0, 1.0, 1.0),5);

	dstCount++;

	//Display image
	imshow("distortion",distortion);

	//Next recursive step
	tryComputeHomography(srcPoints,srcCount,dstPoints,dstCount, src, distortion);
};

//Function can be called to manually examine how well the homography has worked / aligns
void checkHomography(){

	vector<Point2f> cam(4); //source points
	vector<Point2f> proj(4); //computed pojection points

	//Set the points to be projected to be the 4 reference points
	cam[0] = dstPoints[0];
	cam[1] = dstPoints[1];
	cam[2] = dstPoints[2];
	cam[3] = dstPoints[3];

	//Apply the homography
	Mat drawing = Mat::zeros(768, 1024, CV_32FC3);
	perspectiveTransform(cam, proj, H);

	//Draw the 4 points
	for (unsigned int i = 0; i < cam.size(); i++){
		circle(drawing, proj[i], 20, Scalar(255,0,0), -1);
	}

	//Set up the windows
	namedWindow("Bubbles", CV_WINDOW_AUTOSIZE);
	moveWindow("Bubbles", 1680, 0);

	//Display
	imshow("Bubbles", drawing);
	waitKey(10);

}

//Main function of the calibrator(is called in tracking initialisation)
void Calibrator::calibrateCameraProjector(){
	STREAM = s; //kinect or file stream
	
	//Read the homography image chosen
	src = imread( "homographyCorrect.bmp", 1 );
	if(! src.data ) {
		cout <<  "Could not open or find the image" << std::endl ;
    }
	
	//Generate the windows
	namedWindow("source",CV_WINDOW_AUTOSIZE);
	namedWindow("distortion",0); // was CV_WINDOW_AUTOSIZE
	
	//Show and move the image to the projector
	imshow("source",src);
	moveWindow("source", 0, 0); //1680
	setWindowProperty("source", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	waitKey(10);

	//Read a frame from kinect or file
	STREAM->readFrame();

	//Create windows
	imshow("distortion",*(STREAM->_stream));
	
	//Add callbacks(which are like mouse listeners)
	setMouseCallback("source", on_mouse_source, NULL );
	setMouseCallback("distortion", on_mouse_distortion, NULL );
	
	// Set the dst image the same type and size as src
	while (!homographyComputed){
		waitKey(10);
	}

	//checkHomography();
	srcCount = 0;
	dstCount = 0;

	//When the 8 points have been clicked get rid of the calibration windows
	destroyWindow("source");
	destroyWindow("distortion");

	//Set the homography
	_homography = H;
};

