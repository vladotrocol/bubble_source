#include "BubbleTracker.h"



BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){
	_detector = new BubbleDetector(this);
	_detector->init();
	if(_detector->kinect.hasInitialized()){
		_calibrator = new Calibrator(_detector->_stream);
		_calibrator->calibrateCameraProjector();
		_detector->getHomography(&_calibrator->_homography);
		_projector = new ProjectionEngine(_state, _calibrator);
	}
	else{
		cerr<<"Kinect not detected!\nPress any key...\n";
	}
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
				
				d = distanceBetweenPoints(_detector->bubbles[i].center, it->second.center);
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


/*
//Get the minimum distances and the IDs of the closest bubbles
void computeClosestID(map<unsigned int, Bubble>* oldBubbles, BubbleDetector* _detector){
	double min,d;
	unsigned int tempID=0;
	for(unsigned int i=0; i<_detector->bubbles.size(); i++){
		min = 100000;
		for(map<unsigned int, Bubble>::iterator it = oldBubbles->begin(); it != oldBubbles->end(); ++it) {
			d = distanceBetweenPoints(_detector->bubbles[i].center, it->second.center);
			if(min>d&&d!=0){
				min = d;
				tempID = it->second.ID;
			}
		}
		_detector->bubbles[i].closeID = tempID;
		_detector->bubbles[i].minD = min;
	}
};

//Make a decision on which bubble is closest, update and delete it
void decideOnFinalId(map<unsigned int, Bubble>* _old, BubbleDetector* _detector, BubbleState* _state){
	//if(1){
	//	return true;
	//}
	unsigned int detected;
	unsigned int tempI=0,tempID=0;
	double min;
	for(map<unsigned int, Bubble>::iterator it = _old->begin(); it != _old->end();) {
		min = 10000;
		detected = 0;
		for(unsigned int j=0; j<_detector->bubbles.size(); j++){
			if(_detector->bubbles[j].closeID == it->second.ID && min>_detector->bubbles[j].minD){
				min = _detector->bubbles[j].minD;
				tempID = _detector->bubbles[j].closeID;
				tempI = j;
				detected =1;
			}
			if(detected){
				_state->updateBubble(tempID, _detector->bubbles[tempI].center, _detector->bubbles[tempI].radius);
				_detector->bubbles.erase(_detector->bubbles.begin()+tempI);
			}else it++;
		}
	}
	//return decideOnFinalId(_old, _new, _state);
};

void BubbleTracker::update(){
	unsigned int untrID;
	map<unsigned int, Bubble>* trackedBubbles = _state->getCurrentState();

		computeClosestID(trackedBubbles, _detector);
		decideOnFinalId(trackedBubbles, _detector, _state);
					
		//remaining bubbles in new vector must be newly generated ones
		while(_state->hasUnknownBubble()){
				untrID = _state->getUnknownBubble();
				if(untrID){
					_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
					_detector->bubbles.pop_back();
				}
		}
};
*/
void BubbleTracker::printBubbles(){
	_state->printBubbles();
};


