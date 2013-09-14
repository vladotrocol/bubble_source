#include "Bubble.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

// Convert coordinates to milimiters
void Bubble::pixelsToMilimiters(){
	Point3f localCoordinates=_getKinectCoordinates(this->center.x,this->center.y, this->center.z);
	//printf("%.1f %.1f %.1f %f", localCoordinates.x, localCoordinates.y, localCoordinates.z, radius);
	this->center=_fromKinectToWorldCoordinates(localCoordinates);	
	//printf("\t W %.1f %.1f %.1f %f\n", center.x, center.y, center.z, radius);
};

cv::Point3f Bubble::_getKinectCoordinates(float x_p,float y_p, float depth){
	//Shift axes
	float x_k = x_p - 320; //half width
	float y_k = y_p - 240; //half height
	
	float fx = 320/ tan(27.0*M_PI/180);
	float fy = 240/ tan(21.5*M_PI/190);
	//Get kinect coordinates
	return cv::Point3f(x_k * depth / fx, y_k * depth / fy, depth);
	
}
cv::Point3f Bubble::_fromKinectToWorldCoordinates(cv::Point3f p){
	//1. Calculando rotaciones:
	//Midiendo distancia a panel y altura del pnto medio (regla) alpha=atan(51/98)=
	//Midiendo eje Y, usando projector como referencia.alpha=atan(103/202)
	//Midiendo eje Z, usando projector como referencia.alpha=atan(102/214)
 	
	//3. Scaling factors:
	static float  x_scale = 300/273.0f;
	static float  y_scale = 250/228.0f;
	static float  z_scale = 300/281.0f;
	
	//Constant offset (position of the kinect to the projector)
	static float x_off=-8, y_off=185, z_off=30;
	
	//Horizontal rotation
	static float Xangle=0.41f;	//0.465
	//static float angle=atan((51.0/98 + 103.0/202 + 102/214.0)/3);
	Point3f result;
	result.x=x_scale*p.x+x_off;
	result.y=y_scale*(p.y*cos(Xangle)+p.z*sin(Xangle))+y_off;
	result.z=z_scale*(p.y*sin(Xangle)-p.z*cos(Xangle))+z_off;

	//Vertical rotation
	static float Yangle=-0.01;
	Point3f result2;
	result2.x= result.z*sin(Yangle)+ result.x*cos(Yangle);
	result2.y=result.y;
	result2.z=result.z*cos(Yangle) - result.x*sin(Yangle);

	return result2;
}

Bubble::Bubble():center(Point3f(0.0,0.0,0.0)),closeID(0),updated(false){};

void Bubble::write(Point3f newCenter){
	this->center = newCenter;
	updated=true;
};

Point3f Bubble::read(){
	updated=false;
	return this->center;
};