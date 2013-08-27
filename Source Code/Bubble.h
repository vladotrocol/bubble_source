#ifndef _BUBBLE_C
#define _BUBBLE_C

#include <opencv2\opencv.hpp>
#define TRACKED 1
#define NON_TRACKED 0
using namespace cv;

class Bubble {
public:
	Bubble();
	unsigned int ID;
	Point3f center;
	float radius;
	int state;
	unsigned int closeID;
	double minD;
	char* material;
	bool updated;
	void pixelsToMilimiters();
	void write(Point3f center);
	Point3f read();
private :
	inline cv::Point3f _getKinectCoordinates(float x,float y, float depth);
	inline cv::Point3f _fromKinectToWorldCoordinates(cv::Point3f p);
};

#endif