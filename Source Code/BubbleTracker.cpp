#include "BubbleTracker.h"

BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){
	_detector = new BubbleDetector(this);
	_detector->init();
	_calibrator = new Calibrator(_detector->_stream);
	_calibrator->calibrateCameraProjector();
	_projector = new ProjectionEngine(_state, _calibrator);
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

void BubbleTracker::update(){
	map<unsigned int, Bubble>* trackedBubbles = _state->getCurrentState();
	unsigned int untrID;
	double min,d;
	unsigned int tempID=0, tempI=0;
	if(trackedBubbles->size() > 0){
		//Get the minimum distances and the IDs of the closest bubbles
		for(unsigned int i=0; i<_detector->bubbles.size(); i++){
			min = 100000;
			for(map<unsigned int, Bubble>::iterator it = trackedBubbles->begin(); it != trackedBubbles->end(); ++it) {
				d = distanceBetweenPoints(_detector->bubbles[i].center, it->second.center);
				if(min>d){
					min = d;
					tempID = it->second.ID;
				}
			}
			//cout<<tempID<<'\n';
			_detector->bubbles[i].closeID = tempID;
			_detector->bubbles[i].minD = d;
		}

		//make a decision on which bubble is closest, update and delete it
		for(map<unsigned int, Bubble>::iterator it = trackedBubbles->begin(); it != trackedBubbles->end(); ++it) {
				min = 10000;
			for(unsigned int j=0; j<_detector->bubbles.size(); j++){
				if(_detector->bubbles[j].closeID == it->second.ID && min>_detector->bubbles[j].minD){
					min = _detector->bubbles[j].minD;
					tempID = _detector->bubbles[j].closeID;
					tempI = j;
				}
			}
			if(tempI){
				_state->updateBubble(tempID, _detector->bubbles[tempI].center, _detector->bubbles[tempI].radius);
				_detector->bubbles.erase(_detector->bubbles.begin()+tempI);
			}
		}

		//remaining bubbles in new vector must be newly generated ones
		while(_state->hasUnknownBubble()){
			untrID = _state->getUnknownBubble();
			_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
			_detector->bubbles.pop_back();
		}
		//if there are bubbles left they must be noise
		_detector->bubbles.clear();
	}
	else{
		//cout<<"haha";
		while(_state->hasUnknownBubble()){
			untrID = _state->getUnknownBubble();
			_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
			_detector->bubbles.pop_back();
		}

	}
	//printBubbles();
};



void BubbleTracker::printBubbles(){
	_state->printBubbles();
};