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
#include "Filters.h"
#include "Bubble.h"


using namespace std;
using namespace cv;
class Stream{
	public:
		Mat* _stream;
		Filters* filter;
		USHORT* dataMil;
		BYTE* dataPix;
		int minBubbleSize;
		int maxBubbleSize;
		bool TestON;
		Stream(){minBubbleSize=10;maxBubbleSize=100;};
		virtual void readFrame()=0;
		void display(char* s);
		void displayBubbles(vector<Bubble> &bubbles);
		void generateControls();
};
#endif