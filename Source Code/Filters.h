#ifndef _FILTERS_C
#define _FILTERS_C

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


using namespace std;
using namespace cv;

class IFilters{
	public:
		virtual Mat applyFilter(char s, Mat src)=0;
		virtual Mat thresholdFilter(Mat src)=0;
		virtual Mat erosionFilter(Mat src)=0;
		virtual Mat dilationFilter(Mat src)=0;
};


class Filters: public IFilters {
	public:
		int thresholdValue;
		int thresholdType;
		int erosionSize;
		int dilationSize;

		Filters(void);
		Mat applyFilter(char s, Mat src);
		Mat thresholdFilter(Mat src);
		Mat erosionFilter(Mat src);
		Mat dilationFilter(Mat src);
};

#endif