#include <iostream>
#include <vector>
#include <ctime>
//#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
//#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;

struct mouse_info_struct { int x,y; double vx,vy, ax, ay; int time;};
struct mouse_info_struct mouse_info = {-1,-1,0,0,0,0}, last_mouse;
struct _predictor{ struct mouse_info_struct t_1, t_2, prediction;} predictor;
double startTime;
vector<Point> mousev,kalmanv, myPredictor, alphaBetaV;
KalmanFilter KF(6, 4, 0);Mat_<float> measurement(4,1);
static int myTime=0;

//Alpha beta stuff:
float dt = 1;
float xk_1[2] ={ 0,0}, vk_1[2] ={0,0}, a = 0.85, b = 0.005; 
float xk[2], vk[2], rk[2];
float xm[2];

void correct2(){
	xm[0]=mouse_info.x;
	xm[1]=mouse_info.y;
    for(int i=0;i<2;i++)
    {
	   xk[i] = xk_1[i] + ( vk_1[i] * dt );
       vk[i] = vk_1[i];
	   rk[i] = xm[i] - xk[i];
	   xk[i] += a * rk[i];
       vk[i] += ( b * rk[i] ) / dt;
	   xk_1[i] = xk[i];
       vk_1[i] = vk[i];
     }
}

void predict2(){}

void correct(void);

void on_mouse(int event, int x, int y, int flags, void* param) {
	//if (event == CV_EVENT_LBUTTONUP) 
	{
		last_mouse = mouse_info;
		mouse_info.x = x;
		mouse_info.y = y;
		mouse_info.vx=x-last_mouse.x;
		mouse_info.vy=y-last_mouse.y;
		mouse_info.time=myTime;		
//		cout << "got mouse " << x <<","<< y <<endl;
	}
}

void init(){
	predictor.t_2=mouse_info;
	predictor.t_2.time--;
	predictor.t_1=mouse_info;
	predictor.t_1.time++;
	predictor.prediction=predictor.t_1;
	myTime++;
}

float v_correction=0.2;
float a_correction=0.2;
void correct(){
	predictor.t_2=predictor.t_1;
	predictor.t_1=mouse_info;
	//Compute current velocity according to last two measures
	//predictor.t_1.vx=(predictor.t_1.x-predictor.prediction.x)/(myTime-predictor.t_2.time);
	//predictor.t_1.vy=(predictor.t_1.y-predictor.prediction.y)/(myTime-predictor.t_2.time);
	predictor.t_1.vx=(predictor.t_1.x-predictor.prediction.x);
	predictor.t_1.vy=(predictor.t_1.y-predictor.prediction.y);
	if(predictor.t_1.vx==0)
		predictor.t_1.time++;
	else 
		predictor.t_1.time++;
	//Compute predicted velocity:
	
	predictor.prediction.vx=v_correction*predictor.t_1.vx + (1-v_correction)*predictor.prediction.vx;
	predictor.prediction.vy=v_correction*predictor.t_1.vy + (1-v_correction)*predictor.prediction.vy;
	// Correct acceleration from two last oberverd measures
	
	//predictor.prediction.ax=((predictor.t_1.vx-predictor.t_2.vx)*a_correction + (1-a_correction)*predictor.prediction.ax)/(myTime-predictor.t_2.time);
	//predictor.prediction.ay=((predictor.t_1.vy-predictor.t_2.vy)*a_correction + (1-a_correction)*predictor.prediction.ay)/(myTime-predictor.t_2.time);
	predictor.prediction.ax=((predictor.t_1.vx-predictor.t_2.vx)*a_correction + (1-a_correction)*predictor.prediction.ax);
	predictor.prediction.ay=((predictor.t_1.vy-predictor.t_2.vy)*a_correction + (1-a_correction)*predictor.prediction.ay);
	
	predictor.prediction.time=myTime;
}

void predict(double time){
	//double inc_t=(time-predictor.t_2.time);
	//predictor.prediction.x=predictor.prediction.x+predictor.prediction.vx*inc_t+predictor.prediction.ax*inc_t*inc_t/2;
	//predictor.prediction.y=predictor.prediction.y+predictor.prediction.vy*inc_t+predictor.prediction.ay*inc_t*inc_t/2;
	predictor.prediction.x=predictor.prediction.x+predictor.prediction.vx+predictor.prediction.ax;
	predictor.prediction.y=predictor.prediction.y+predictor.prediction.vy+predictor.prediction.ay;
}

int main (int argc, char * const argv[]) {
    
	
	Mat img(500, 500, CV_8UC3);
    
    Mat_<float> state(6, 1); /* (x, y, Vx, Vy,Ax, Ay) */
    Mat processNoise(6, 1, CV_32F);
    measurement.setTo(Scalar(0));
    char code = (char)-1;
	
	namedWindow("mouse kalman");
	setMouseCallback("mouse kalman", on_mouse, 0);
	
    for(;;)
    {
		myTime+=2;

        KF.statePre.at<float>(0) = mouse_info.x;
		KF.statePre.at<float>(1) = mouse_info.y;
		KF.statePre.at<float>(2) = mouse_info.vx;
		KF.statePre.at<float>(3) = mouse_info.vy;
		KF.transitionMatrix = *(Mat_<float>(6, 6) << 1,0,1,0,1,0,   0,1,0,1,0,1,  0,0,1,0,1,0,  0,0,0,1,0,1,  0,0,0,0,1,0,  0,0,0,0,0,1);
		//KF.transitionMatrix = *(Mat_<float>(3, 3) << 1,1,1,   0,1,1,  0,0,1);
        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
        setIdentity(KF.errorCovPost, Scalar::all(.01));
		
		mousev.clear();
		kalmanv.clear();
        //randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
		int i=1;
        for(;;)
        {

			myTime++;
//			Point statePt(state(0),state(1));
			//i--;
			if(i==0){
				KF.statePre.at<float>(0) = mouse_info.x;
				KF.statePre.at<float>(1) = mouse_info.y;
				KF.statePre.at<float>(2) = mouse_info.vx;
				KF.statePre.at<float>(3) = mouse_info.vy;
				//KF.transitionMatrix = *(Mat_<float>(3, 3) << 1,1,1,   0,1,1,  0,0,1);
				KF.transitionMatrix = *(Mat_<float>(6, 6) << 1,0,1,0,1,0,   0,1,0,1,0,1, 0,0,1,0,1,0, 0,0,0,1,0,1, 0,0,0,0,1,0,  0,0,0,0,0,1 );
				setIdentity(KF.measurementMatrix);
				setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
				setIdentity(KF.measurementNoiseCov, Scalar::all(0.005f));
				setIdentity(KF.errorCovPost, Scalar::all(.1));
				init();
				mousev.clear();
				kalmanv.clear();
				myPredictor.clear();
				alphaBetaV.clear();
				xk_1[0]=0; xk_1[1]=0;vk_1[0] =0; vk_1[1]=0;
				myTime=0;
				i=50;
			}
            //Predecir
			setIdentity(KF.measurementMatrix,0);
			KF.correct(measurement);
			Mat prediction = KF.predict();

			predict(myTime);  
			predict2();
			printf("Kalman (%d, %d)\n", prediction.at<float>(0), prediction.at<float>(1));

            
			//Pintar		
			Point statePt(mouse_info.x,mouse_info.y);
			Point kalmanPrediction(prediction.at<float>(0),prediction.at<float>(1));
			Point myPredictedPoint(predictor.prediction.x,predictor.prediction.y);
			Point alphaBetaPrediction(xk[0], xk[1]);
			

			myPredictor.push_back(myPredictedPoint);
			kalmanv.push_back(kalmanPrediction);
			mousev.push_back(statePt);
			alphaBetaV.push_back(alphaBetaPrediction);
            
			
            // plot points
#define drawCross( center, color, d )                                 \
line( img, Point( center.x - d, center.y - d ),                \
Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ),                \
Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

            img = Scalar::all(0);
            drawCross( statePt,  Scalar(255,255,255), 5 );
            drawCross( kalmanPrediction,Scalar(0,0,255), 5 );
			drawCross( myPredictedPoint, Scalar(255,0,0), 5 );
			drawCross( alphaBetaPrediction, Scalar(255,255,0), 5 );

			
			
			statePt.y=440;
			kalmanPrediction.y=455;
			myPredictedPoint.y=470;
			alphaBetaPrediction.y=485;
			drawCross( statePt, Scalar(255,255,255), 5 );
			drawCross( kalmanPrediction, Scalar(0,0,255), 5 );
			drawCross( myPredictedPoint, Scalar(255,0,0), 5 );
			drawCross( alphaBetaPrediction, Scalar(255,255,0), 5 );

//          drawCross( predictPt, Scalar(0,255,0), 3 );
//			line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
//			line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );
			//Show target path
			for (int i = 0; i < mousev.size()-1; i++) {
				line(img, mousev[i], mousev[i+1], Scalar(255,255,255), 1);
			}
			//Show filters path
			for (int i = 0; i < kalmanv.size()-1; i++) {
				line(img, kalmanv[i], kalmanv[i+1], Scalar(0,0,255), 1);
			}
			for (int i = 0; i < myPredictor.size()-1; i++) {
				line(img, myPredictor[i], myPredictor[i+1], Scalar(255,0,0), 1);
			}
			for (int i = 0; i < alphaBetaV.size()-1; i++) {
				line(img, alphaBetaV[i], alphaBetaV[i+1], Scalar(255,255,0), 1);
			}
//            randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
//            state = KF.transitionMatrix*state + processNoise;
			static int correctIn=5;
			correctIn--;
			if(correctIn==0){
				//Correct once in five measures.
				measurement(0) = mouse_info.x;
				measurement(1) = mouse_info.y;
				measurement(2) = mouse_info.vx;
				measurement(3) = mouse_info.vy;
				// generate measurement
				//measurement += KF.measurementMatrix*state;
				setIdentity(KF.measurementMatrix);
				Mat estimated = KF.correct(measurement);
				KF.predict();
				correct();
				correct2();
				correctIn=3;
			}

            imshow( "mouse kalman", img );
            code = (char)waitKey(100);
			
            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }
	
    return 0;
}


