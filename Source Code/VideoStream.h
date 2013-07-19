#ifndef _VIDEOPLAYER_C
#define _VIDEOPLAYER_C

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
#include "Filters.h"
#include "Stream.h"

class VideoStream: Stream{
	public:
		VideoStream(char* fileNam);
		virtual void readFrame();
		Mat	depth_src;
		char* fileName;
		VideoCapture* cap;
};
#endif
