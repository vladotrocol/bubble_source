#ifndef _BUBBLE_C
#define _BUBBLE_C

#include <opencv2\opencv.hpp>
#include "Predictor.h"
#define TRACKED true
#define NON_TRACKED false
using namespace cv;

class Bubble {
public:
	Bubble();
	Predictor predict;
	unsigned int ID;
	Point3f center;
	float radius;
	bool state;
	unsigned int closeID;
	double minD;
	string material;
	bool updated;
	void pixelsToMilimiters();
	void write(Point3f center);
	Point3f read();
	float setRadius;
private :
	inline cv::Point3f _getKinectCoordinates(float x,float y, float depth);
	inline cv::Point3f _fromKinectToWorldCoordinates(cv::Point3f p);
};

#endif