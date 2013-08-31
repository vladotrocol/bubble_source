#ifndef _PREDICTOR
#define _PREDICTOR

#include <iostream>
#include <vector>
#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#define NO_POINTS 0
#define ONE_POINT 1
#define TWO_POINTS 2
#define READY 3

using namespace cv;

class Predictor{
	public:
		Mat* img;
		unsigned char status;
		Predictor();
		void init(bool display);
		void correct(Point3f center);
		Point3f predict();
		void display();
		void test();

	private:
		float v_c;
		float a_c;

		Point3f P0, P1, P2;
		Point3f v0,v1;
		Point3f a0;

		int t0,t1,t2;
		float CT, PT;

		int   cST, //correct starting time 
			  cNT,
			  pNT; //correct new time
};

#endif