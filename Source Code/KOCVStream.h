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
#include "Stream.h"

using namespace std;
using namespace cv;



class KOCVStream: public Stream{
	public:
		Kinect* kinect;
		char options;

		KOCVStream(); //Constructor
		virtual void readFrame(); //Reads the current frames into the source materials
		void record(char* fileName);

	private:
		unsigned int static const width = 640;
		unsigned int static const height = 480;

		Mat tryReadFrame(); //Read the Mat data into a image frame
		NUI_IMAGE_FRAME tryRecordFrame(char s); //Read the Mat data into a image frame
		Mat kFrameToMat(NUI_IMAGE_FRAME imageFrame); //Put the kinect frame data onto a Mat
};
#endif