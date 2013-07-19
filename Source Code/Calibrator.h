#ifndef _CALIBRATOR_C
#define _CALIBRATOR_C
#include "KOCVStream.h"
#include <opencv2\opencv.hpp>

using namespace cv;

class Calibrator{
	public:
		Mat _homography;
		Stream* s;
		Calibrator(Stream* stream);
		void calibrateCameraProjector();
};

#endif