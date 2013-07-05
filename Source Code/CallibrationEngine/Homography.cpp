#include "./Homography.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "./WaterDisplayUSBController/WDDLayer.h"
#include <iostream>
#include <stdio.h>

void on_mouse_source(int evt, int x, int y, int flags, void* param);
void on_mouse_distortion(int evt, int x, int y, int flags, void* param);

using namespace cv;

static Point2f _srcPoints[4];
static Point2f _dstPoints[4];
static int srcCount=0;
static int dstCount=0;
Mat contents, distortion, result;
std::string cameraWindowName="";

/**
	@evt: One of CV_EVENT_*
	@x and y: Coordinates of mouse pointer in image coordinates (not window coordinates)
	@flags:	Combination of CV_EVENT_FLAG
	@param:	User-defined parameter passed to the cvSetMouseCallback function call
*/
void on_mouse_source(int evt, int x, int y, int flags, void* param){
    if( evt != CV_EVENT_LBUTTONDOWN )
        return;
    if(srcCount>=4)
        return;
    _srcPoints[srcCount]=Point2f(x,y);
    // circle(img, center, radius, color, thickness, lineType, shift)
    circle(distortion, _srcPoints[srcCount], 5, Scalar(1.0, 0.0, 0),5);
    imshow(cameraWindowName,distortion);
    srcCount++;
    //tryComputeHomography(srcPoints,srcCount,dstPoints,dstCount, src, distortion);
}


void on_mouse_contents(int evt, int x, int y, int flags, void* param){ 

	if(dstCount>=4)
        return;
	 static int count=0;
     ///contents=Mat::ones( 608, 640, contents.type() );  
	 cv::rectangle(contents,cv::Rect(0,0,608,684),cv::Scalar(255,255,255),-1);
	 switch(evt){
		 case CV_EVENT_LBUTTONDOWN :
			 _dstPoints[dstCount]=Point2f(x,y);
			 
			 dstCount++;
			 break;
		 case CV_EVENT_MOUSEMOVE:
			 count=(count+1)%512;
			 cv::rectangle(contents,cv::Rect(x-2,y-10,4,20), cv::Scalar(0,0,0), -1);
			 cv::rectangle(contents,cv::Rect(x-10,y-2,20,4), cv::Scalar(0,0,0), -1);
			 break;
	 }
	 
	 for(int i=0;i<dstCount;i++)
		circle(contents, _dstPoints[i], 3, Scalar(80, 80, 80),5);
	 imshow("projector",contents);
	 imshow("contents",contents);
	 waitKey(1);
	 //tryComputeHomography(srcPoints,srcCount,dstPoints,dstCount, src, distortion);
}


void Homography::selectHomographyPoints(std::string sourceWindow, Mat& cameraImage)
{
	cameraWindowName=sourceWindow;
	// Checkerboard image
	contents = Mat::zeros( 684, 608, cameraImage.type() ); 
	// What the camera sees
	distortion = cameraImage;
	//Create windows
	namedWindow("contents",CV_WINDOW_AUTOSIZE);
	//namedWindow(sourceWindow.c_str(),CV_WINDOW_AUTOSIZE);//This is already created...
	namedWindow("projector",CV_WINDOW_AUTOSIZE);
	//Add callbacks
	cvSetMouseCallback(sourceWindow.c_str(), on_mouse_source, NULL);
	cvSetMouseCallback("contents", on_mouse_contents, NULL);
	while(srcCount<4 || dstCount<4){
		imshow("contents",contents);
		imshow(sourceWindow.c_str(),distortion);
		imshow("projector",contents);
		cv::waitKey(1);
	}
    //When presenting the window in the lightcrafter screen, there is an offset due to the border of the window.
	//We took a screenshot and measured the amount of pixels of that offset
	int offset_x=13, offset_y=0;//-35;
	for(int i=0;i<4;i++){
		_dstPoints[i].x+=offset_x;
		_dstPoints[i].y+=offset_y;
	}
	//Copy temporary values to my atributes to prevent overwriting
	memcpy(this->srcPoints, _srcPoints, sizeof(_srcPoints));
	memcpy(this->dstPoints, _dstPoints, sizeof(_dstPoints));
	/* //This does not look right any longer(homography is computer from srcPoints to (0,0)(608, 640), the correction of dstPoints is applied in the OGRE program)
	Mat aux,aux2=Mat::zeros( contents.size[0],contents.size[1], contents.type() );
	distortion.copyTo(aux);
	this->applyHomographyToStencil(aux,aux2);
	cv::add(aux2,contents,aux2);
	imshow("contents",aux2);
	waitKey(0);*/
	//Destroy windows:
	cv::destroyWindow("contents");
	cv::destroyWindow("projector");
	srcCount=0;
	dstCount=0;
	
}


void Homography::applyHomographyToStencil(Mat& stencil, Mat& correctedStencil){
	Mat H;
	int w,h;
	w=correctedStencil.size[0];
	h=correctedStencil.size[1];
	cv::Point2f wholeDstImg[4]={cv::Point2f(0,0),cv::Point2f(w,0),cv::Point2f(w,h),cv::Point2f(0,h)};
	// Calculate the perspective transform from 4 pairs of corresponding points
	H=getPerspectiveTransform(srcPoints,wholeDstImg); 
	// Create a zeroed matrix the same size as the distortion matrix
	correctedStencil=Mat::zeros( correctedStencil.size[0],correctedStencil.size[1], correctedStencil.type() ); 
	// Apply the perspective transformation to the 'result' matrix
	warpPerspective( stencil, correctedStencil, H, correctedStencil.size() ); 
	//cvSetWindowProperty("distortion", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
	//cvSetWindowProperty("source", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
	return;
}


void Homography::setSrcPoint(int pointId, Point2f src){
	srcPoints[pointId]=src;
}

void Homography::setDstPoint(int pointId, Point2f dst){
	dstPoints[pointId]=dst;
}



Homography::Homography(float srcPoints[4][2],float dstPoints[4][2]){

	this->srcPoints[0].x = (float)srcPoints[0][0];
	this->srcPoints[0].y = (float)srcPoints[0][1];
	this->srcPoints[1].x = (float)srcPoints[1][0];
	this->srcPoints[1].y = (float)srcPoints[1][1];
	this->srcPoints[2].x = (float)srcPoints[2][0];
	this->srcPoints[2].y = (float)srcPoints[2][1];
	this->srcPoints[3].x = (float)srcPoints[3][0];
	this->srcPoints[3].y = (float)srcPoints[3][1];

	this->dstPoints[0].x = (float)dstPoints[0][0];
	this->dstPoints[0].y = (float)dstPoints[0][1];
	this->dstPoints[1].x = (float)dstPoints[1][0];
	this->dstPoints[1].y = (float)dstPoints[1][1];
	this->dstPoints[2].x = (float)dstPoints[2][0];
	this->dstPoints[2].y = (float)dstPoints[2][1];
	this->dstPoints[3].x = (float)dstPoints[3][0];
	this->dstPoints[3].y = (float)dstPoints[3][1];
}