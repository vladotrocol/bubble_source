#include "BubbleTracker.h"

BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){
	_detector = new BubbleDetector(this);
	_detector->init();
	_calibrator = new Calibrator(_detector->STREAM);
	return true;
};

bool BubbleTracker::start(){
	_detector->start();
	_detector->run();
	return true;
};

bool BubbleTracker::stop(){
	_detector->stop();
	return true;
};

void BubbleTracker::update(float x, float y, float radius){
	unsigned int untrID = _state->getUnknownBubble();
	if(untrID){
		_state->updateBubble(untrID, x, y, radius);
	}
};