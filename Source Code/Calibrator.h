#ifndef _CALIBRATOR_C
#define _CALIBRATOR_C
#include "KOCVStream.h"
#include <opencv2\opencv.hpp>

using namespace cv;

class Calibrator{
	public:
		KOCVStream* s;
		Calibrator(KOCVStream& stream);
		void calibrateCameraProjector();
};

#endif