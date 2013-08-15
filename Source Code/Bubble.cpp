#include "Bubble.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

// Convert coordinates to milimiters
Point3f Bubble::pixelsToMilimiters(){
	//Shift axes
	float x = this->center.x - 320; //half width
	float y = 240 - this->center.y; //half height
	
	float fx = 320/ tan(28.5*M_PI/180);
	float fy = 240/ tan(21.5*M_PI/190);
	
	float X = x * this->center.z / fx;
	float Y = y * this->center.z / fy;
	float Z = this->center.z;
	printf("%f %f %f\n", X, Y, Z);
	return Point3f(X, Y, Z);
};