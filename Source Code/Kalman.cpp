#include <iostream>
#include <vector>
#include <ctime>
//#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
//#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;
Mat img(800,1200, CV_8UC3);
struct mouse_info_struct { float x,y; float vx,vy, ax, ay; float time;};
struct mouse_info_struct mouse_info = {-1,-1,0,0,0,0}, last_mouse;
struct _predictor{ struct mouse_info_struct t_1, t_2, prediction;} predictor;
vector<Point> mousev,kalmanv, myPredictor, alphaBetaV;
clock_t t;
bool initi=false;
int inita = 0;
void correct(void);


int r=0;


int sPT, 
	sCT, 
	nPT,
	nCT,
	S;

float 	
	dPT,
	dCT,
	PT,
	CT,
	_t;

void feedData() {
		if(!initi){
			initi=true;
		}
		
		
		nCT = clock();
		dCT = (float)(nCT-sCT)/CLOCKS_PER_SEC;
		if(dCT>0.612){
			inita++;
			sCT=nCT;
			CT+=dCT;
			last_mouse = mouse_info;
			mouse_info.x = (float)_t*50;
			mouse_info.y = (float)sin(_t)*50+400;
			mouse_info.vx= (float)_t*50-last_mouse.x;
			mouse_info.vy=(float)sin(_t)*50+400-last_mouse.y;
			r=0;
			mouse_info.time=CT;
			PT=0;
			circle( img,Point2f(mouse_info.x,mouse_info.y),1,Scalar( 255, 0, 0 ),-1,8 );
			if(initi&&inita>2){
				correct();
			}
		}
}

void init(){
	predictor.t_2=mouse_info;
	predictor.t_2.time--;
	predictor.t_1=mouse_info;
	predictor.t_1.time++;
	predictor.prediction=predictor.t_1;
}

float v_correction=0.2;
float a_correction=0.2;

void correct(){
	predictor.t_2=predictor.t_1;
	predictor.t_1=mouse_info;
	//Compute current velocity according to last two measures
	predictor.t_1.vx=(predictor.t_1.x-predictor.prediction.x)/(PT-predictor.t_2.time);
	predictor.t_1.vy=(predictor.t_1.y-predictor.prediction.y)/(PT-predictor.t_2.time);
	if(predictor.t_1.vx==0)
		predictor.t_1.time++;
	else 
		predictor.t_1.time++;
	//Compute predicted velocity:
	
	predictor.prediction.vx=v_correction*predictor.t_1.vx + (1-v_correction)*predictor.prediction.vx;
	predictor.prediction.vy=v_correction*predictor.t_1.vy + (1-v_correction)*predictor.prediction.vy;
	// Correct acceleration from two last oberverd measures
	
	predictor.prediction.ax=((predictor.t_1.vx-predictor.t_2.vx)*a_correction + (1-a_correction)*predictor.prediction.ax)/(PT-predictor.t_2.time);
	predictor.prediction.ay=((predictor.t_1.vy-predictor.t_2.vy)*a_correction + (1-a_correction)*predictor.prediction.ay)/(PT-predictor.t_2.time);
	
	predictor.prediction.time=PT;
}

void predict(float time){
	float inc_t=(time-predictor.t_2.time);
	predictor.prediction.x=predictor.prediction.x+predictor.prediction.vx*inc_t+predictor.prediction.ax*inc_t*inc_t/2;
	predictor.prediction.y=predictor.prediction.y+predictor.prediction.vy*inc_t+predictor.prediction.ay*inc_t*inc_t/2;
}


int main (int argc, char * const argv[]) {

	sPT = clock();
	sCT = clock();
	S = clock();
	init();
	namedWindow("Prediction Filter");
	cvMoveWindow( "Prediction Filter", 0, 0 );
	cvResizeWindow(  "Prediction Filter", 1200, 800);
	//setMouseCallback("Prediction Filter", on_mouse, 0);
	while(1){
		nPT = clock();
		dPT = (float)(nPT - sPT)/CLOCKS_PER_SEC;
		S = clock()-S;
		_t = ((float) S)/CLOCKS_PER_SEC;
		if(initi && dPT>0.1&&inita>2){
			PT+=dPT;
			predict(PT);
			sPT=nPT;
			circle( img,Point2f(predictor.prediction.x,predictor.prediction.y),1,Scalar( 0, r+=40, 255-r ),-1,8 );
			//printf("1 ");
		}
		feedData();
			imshow("Prediction Filter", img);
			waitKey(1);
	}
    return 0;
}


