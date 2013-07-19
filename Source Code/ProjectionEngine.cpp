#include "ProjectionEngine.h"

ProjectionEngine::ProjectionEngine(BubbleState* BS, Calibrator* CA):_state(BS), _calibrator(CA){;};



//OLDER VERSION OF THE FUNCTION: HOMOGRAPHY SHOULD NOT HAPPEN HERE
void ProjectionEngine::Draw(){
	Mat drawing = Mat::zeros(768, 1024, CV_32FC3);
	map<unsigned int, Bubble>* _curState = _state->getCurrentState();
	float radius;
	if(_curState->size()>0){
		for(map<unsigned int, Bubble>::iterator it = _curState->begin(); it != _curState->end(); ++it) {
			radius = it->second.radius;
			if(radius>0){
				circle(drawing, Point2f(it->second.center.x,it->second.center.y), radius, Scalar(0,255,0), -1);
			}
		}
	}

	namedWindow("Bubbles2", CV_WINDOW_AUTOSIZE);
	moveWindow("Bubbles", 1680, 0);
	setWindowProperty("Bubbles", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);

	imshow("Bubbles2", drawing);
	waitKey(10);

};