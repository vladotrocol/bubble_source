#ifndef _HOMOGRAPHY
#define _HOMOGRAPHY
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "./WaterDisplayUSBController/WDDLayer.h"
#include <iostream>
#include <stdio.h>

using namespace cv;

class Homography{

	Point2f srcPoints[4];
	Point2f dstPoints[4];

	public:
		//Start a new homography
		Homography(){// 1:1 homography (no change)
			srcPoints[0]=dstPoints[0]=Point2f(1,1);
			srcPoints[3]=dstPoints[3]=Point2f(1,10);
			srcPoints[1]=dstPoints[1]=Point2f(10,10);
			srcPoints[2]=dstPoints[2]=Point2f(10,1);
		}
		//Load the saved homography points from the 'calibration.cfg' file
		Homography(float srcPoints[4][2],float dstPoints[4][2]);
		//You get control until you have the eight points. At the end of the method copy srcPoints and dstPoints to your local variables
		void selectHomographyPoints(std::string sourceWindow, Mat& source);
		//Use your homography to correct this stencil
		void applyHomographyToStencil(Mat& stencil, Mat& correctedStencil);
		//Save a point in the destination image
		inline void setDstPoint(int pointID, Point2f dstPoint);
		//Fill the points array with the saved homography points for that layer
		inline void setSrcPoint(int pointID, Point2f srcPoint);
		// Get the homography points 
		inline cv::Point2f getDstPoint(int pointID){
			return dstPoints[pointID];
		}
		// Get the homography points 
		inline cv::Point2f getSrcPoint(int pointID){
			return srcPoints[pointID];
		}

};

#endif