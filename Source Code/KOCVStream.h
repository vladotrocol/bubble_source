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
		Mat rgb_src,
			depth_src;
		Kinect* kinect;
		Filters* filter;
		char options;

		KOCVStream(Kinect* kinect, 	Filters* filter, char opts); //Constructor
		//void display(char* s); //Display streams in windows
		virtual void readFrame(); //Reads the current frames into the source materials
		//void displayBubbles(vector<Bubble>& bubbles); //Display detected bubbles as circles
		void record(char* fileName);

	private:
		unsigned int static const width = 640;
		unsigned int static const height = 480;

		Mat tryReadFrame(char s); //Read the Mat data into a image frame
		NUI_IMAGE_FRAME tryRecordFrame(char s); //Read the Mat data into a image frame
		Mat kFrameToMat(char s,NUI_IMAGE_FRAME imageFrame); //Put the kinect frame data onto a Mat
		BYTE* whichImageData(char s, NUI_LOCKED_RECT* LockedRect); //Get the image data correspondig to flag
		int whichDataType(char s); //This function returns the correct data format for each stream type
		Mat* whichSource(char s); //Returns the correct source
		void generateWindows(char* s); //Generates windows according to grammar
		void generateControls(void); //Generates the window with the filter sliders 
		
};
#endif