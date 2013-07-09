#include "BubbleTracker.h"



BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){

	_detector = new BubbleDetector(this);
	_detector->init();

	if(_detector->kinect.hasInitialized()){
		_calibrator = new Calibrator(_detector->_stream);
		_calibrator->calibrateCameraProjector();
		_detector->getHomography(_calibrator->_homography);
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

void BubbleTracker::update(){
	map<unsigned int, Bubble>* trackedBubbles = _state->getCurrentState();
	unsigned int untrID, detected;
	double min,d;
	unsigned int tempID=0, tempI=0;
	if(trackedBubbles->size() > 0&&_detector->bubbles.size()>0){
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
			_detector->bubbles[i].closeID = tempID;
			_detector->bubbles[i].minD = d;
			//make a decision on which bubble is closest, update and delete it
			for(map<unsigned int, Bubble>::iterator it = trackedBubbles->begin(); it != trackedBubbles->end(); ++it) {
					min = 10000;
					detected = 0;
				for(unsigned int j=0; j<_detector->bubbles.size(); j++){
					if(_detector->bubbles[j].closeID == it->second.ID && min>_detector->bubbles[j].minD){
						min = _detector->bubbles[j].minD;
						tempID = _detector->bubbles[j].closeID;
						tempI = j;
						detected =1;
					}
				}
					if(detected){
					_state->updateBubble(tempID, _detector->bubbles[tempI].center, _detector->bubbles[tempI].radius);
					//cout<<tempID<<'\n';
					//_detector->bubbles.erase(_detector->bubbles.begin()+tempI);
					}
			}
		}

		//remaining bubbles in new vector must be newly generated ones
		while(_state->hasUnknownBubble()){
			untrID = _state->getUnknownBubble();
			if(untrID&&_detector->bubbles.size()>0){
				_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
				_detector->bubbles.pop_back();
			}
		}
	}
	else{
		while(_state->hasUnknownBubble()){
			untrID = _state->getUnknownBubble();
			if(untrID&&_detector->bubbles.size()>0){
				_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius);
				_detector->bubbles.pop_back();
			}
		}

	}
	//printBubbles();
	//_projector->Draw();
};



void BubbleTracker::printBubbles(){
	_state->printBubbles();
};