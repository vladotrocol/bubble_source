#ifndef _STREAM_C
#define _STREAM_C

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
//#include "KOCVStream.h"
//#include "VideoStream.h"

using namespace std;
using namespace cv;
class Stream{
	public:
		Mat* _stream;
		//KOCVStream* ks;
		//VideoStream* vs;
		bool TestON;
		Stream();
		Stream* getParent();
		virtual void readFrame()=0;
};
#endif