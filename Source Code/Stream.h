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
#include "Filters.h"
#include "Bubble.h"
#include "BubbleState.h"
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
		virtual void displayBubbles(vector<Bubble> &bubbles);
		virtual void displayTrackedBubbles(BubbleState* _state);
		void generateControls();
};
#endif