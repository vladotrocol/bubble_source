#include "BubbleTracker.h"



BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){
	_detector = new BubbleDetector(this);
	_detector->init();
		_calibrator = new Calibrator(_detector->_capture);
		_calibrator->calibrateCameraProjector();
		_detector->getHomography(&_calibrator->_homography);
	return true;
};

bool BubbleTracker::start(){
	_detector->start();
	return true;
};

bool BubbleTracker::stop(){
	_detector->stop();
	return true;
};

double distanceBetweenPoints(Point2f a, Point2f b){
	double res = pow((double)b.x - a.x, 2.0) + pow((double)b.y - a.y, 2.0);
	return sqrt(res);
};



bool assignID(map<unsigned int, Bubble>::iterator iter,
				BubbleDetector* _detector,
				BubbleState* _state, 
				map<unsigned int, Bubble>* trackedBubbles){


	double min,d;
	unsigned int tempID=0, untrID=0, detected=0, tempI=0;
	
	while(_state->hasUnknownBubble()){
		if(_detector->bubbles.size()>0){
			untrID = _state->getUnknownBubble();
		}
		else{
			break;
		}
		if(untrID){
			_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
			_detector->bubbles.pop_back();
		}
	}

	if(iter == trackedBubbles->end()){
		return true;
	}

	if(_detector->bubbles.size()>0&&trackedBubbles->size()>0){
		for(unsigned int i=0; i<_detector->bubbles.size(); i++){
			min = 100000;
			detected = 0;
			for(map<unsigned int, Bubble>::iterator it = iter; it != trackedBubbles->end(); ++it) {
				
				d = distanceBetweenPoints(Point2f(_detector->bubbles[i].center.x, _detector->bubbles[i].center.y), Point2f(it->second.center.x,it->second.center.y));
				if(min>d&&d!=0){
					min = d;
					tempID = it->first;
					detected =1;
				}
			}
			if(detected){
				_detector->bubbles[i].closeID = tempID;
				_detector->bubbles[i].minD = min;
			}
		}

		min = 10000;
		detected = 0;

		for(unsigned int j=0; j<_detector->bubbles.size(); j++){
			if(_detector->bubbles[j].closeID == iter->first && min>_detector->bubbles[j].minD){
				min = _detector->bubbles[j].minD;
				tempID = _detector->bubbles[j].closeID;
				tempI = j;
				detected =1;
			}
		}
			if(detected){
				_state->updateBubble(tempID, _detector->bubbles[tempI].center, _detector->bubbles[tempI].radius);
				_detector->bubbles.erase(_detector->bubbles.begin()+tempI);
			}
	}


		iter++;
		assignID(iter, _detector, _state, trackedBubbles);

};

void BubbleTracker::update(){
	map<unsigned int, Bubble>* trackedBubbles = _state->getCurrentState();
	map<unsigned int, Bubble>::iterator iter = trackedBubbles->begin();
	assignID(iter, _detector, _state, trackedBubbles);
};

void BubbleTracker::printBubbles(){
	_state->printBubbles();
};


