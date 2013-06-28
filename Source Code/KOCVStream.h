#ifndef _KOCVSTREAM_C
#define _KOCVSTREAM_C

#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include "Kinect.h"
#include "Filters.h"
#include "BubbleState.h"

#define width 640 
#define height 480

using namespace std;
using namespace cv;

class KOCVStream{
	public:
		Mat rgb_src,
			depth_src;
		Kinect kinect;
		Filters filter;

		//Constructor
		KOCVStream(Kinect& kinect, 	Filters& filter);

		//External Api
		void display(char* s);
		void readFrame(char s);
		void displayBubbles(vector<Bubble>& bubbles);

	private:
		//Internal
		Mat tryReadFrame(char s);
		Mat kFrameToMat(char s,NUI_IMAGE_FRAME imageFrame);
		BYTE* whichImageData(char s, NUI_LOCKED_RECT* LockedRect);
		int whichDataType(char s);
		Mat* whichSource(char s);
		void generateWindows(char* s);
		void generateControls(void);	
};

#endif