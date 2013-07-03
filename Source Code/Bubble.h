#ifndef _BUBBLE_C
#define _BUBBLE_C

#include <opencv2\opencv.hpp>
#define TRACKED 1
#define NON_TRACKED 0
using namespace cv;

class Bubble {
public:
	unsigned int ID;
	Point2f center;
	float radius;
	int state;
	unsigned int closeID;
	double minD;
};

#endif