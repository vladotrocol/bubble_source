#include "BubbleTracker.h"

BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

//Initialise the bubble tracker
bool BubbleTracker::init(){
	_detector = new BubbleDetector(this, _state);
	_detector->init();
	//-------------------Calibrate(this or read form file)-------------
		/*_calibrator = new Calibrator(_detector->_capture);
		_calibrator->calibrateCameraProjector();
		_detector->getHomography(&_calibrator->_homography);*/
	
	//If already calibrated
		_detector->readHomography();
	return true;
};

//Start the detection on it's own thread
bool BubbleTracker::start(){
	_detector->start();
	return true;
};

//Kill the application
bool BubbleTracker::stop(){
	_detector->stop();
	return true;
};

//Compute the distance between two points
double distanceBetweenPoints(Point2f a, Point2f b){
	double res = pow((double)b.x - a.x, 2.0) + pow((double)b.y - a.y, 2.0);
	return sqrt(res);
};

map<int, int> lastSeen;


//Add non-tracked bubbles to tracked
void BubbleTracker::insertUntracked(map<unsigned int, Bubble>* trackedBubbles){
		//------------------------------------------------------------------------
	//if a bubble is detected, there are less tracked bubbles than detected ones
	//and there are still some untracked bubbles in the queue
	//insert the bubbles into the database
	//cout<<"Size: "<<_detector->bubbles.size()<<'\n';
	for(unsigned int i=0; i<_detector->bubbles.size(); i++){
		if(_state->hasUnknownBubble()&&trackedBubbles->size()<=_detector->bubbles.size()){
				int untrID=0;
				untrID = _state->getUnknownBubble();
				if(untrID){
					_state->updateBubble(untrID, _detector->bubbles.back().center, _detector->bubbles.back().radius, true);
					lastSeen[untrID] = 30;
					_detector->bubbles.pop_back();
				}
			}
	}
};

void BubbleTracker::computeCloseIds(map<unsigned int, Bubble>::iterator iter,map<unsigned int, Bubble>* trackedBubbles){
	int min;
	int detected;
	unsigned int tempID=0;
	double d;

		//-------------Compute the closest bubble to each bubble-------------------------------
		for(unsigned int i=0; i<_detector->bubbles.size(); i++){
			min = 100000;
			detected = 0;
			for(map<unsigned int, Bubble>::iterator it = iter; it != trackedBubbles->end(); ++it) {
				
				//Distance between a tracked bubbles and a detected one
				d = distanceBetweenPoints(Point2f(_detector->bubbles[i].center.x, _detector->bubbles[i].center.y),
											Point2f(it->second.center.x, it->second.center.y));
				//Compute minimum distance
				if(min>d){
					min = d;
					tempID = it->first;
					detected =1;
				}
			}
			//If a minimum distance is found store it and the id of the closest bubble 
			if(detected){
				_detector->bubbles[i].closeID = tempID;
				_detector->bubbles[i].minD = min;
			}
		}

};

//Recursive function which tracks the ids of the bubbles
bool BubbleTracker::assignID(map<unsigned int, Bubble>::iterator iter,
				BubbleDetector* _detector,
				BubbleState* _state, 
				map<unsigned int, Bubble>* trackedBubbles){

	double min,d;
	unsigned int tempID=0, detected=0, tempI=0;
	//printf("%d ", trackedBubbles->size());
	if(_detector->bubbles.size()>0&&trackedBubbles->size()>0){
	
		detected = 0;
		min = 10000;
		//--------------------------------Assign the bubble ids--------------------------------
		for(unsigned int j=0; j<_detector->bubbles.size(); j++){
			if(_detector->bubbles[j].closeID == iter->first && min>_detector->bubbles[j].minD){
				min = _detector->bubbles[j].minD;
				tempID = _detector->bubbles[j].closeID;
				tempI = j;
				detected =1;
			}
		}

		if(detected){
			if(lastSeen[tempID]<50){
				lastSeen[tempID]+=2;
			}
			_state->updateBubble(tempID, _detector->bubbles[tempI].center, _detector->bubbles[tempI].radius, true);
			_detector->bubbles.erase(_detector->bubbles.begin()+tempI);
		}
	}

	//-------------------------------Bubble popping----------------------------------
	if(trackedBubbles->size()>0){
		if(!detected&&iter->second.state){
			lastSeen[iter->first]--;
		}

		if(lastSeen[iter->first]==-1){
			int a = iter->first;
			_state->notifyUserBreaks(a);
			lastSeen.erase(a);
		}
		iter++;
	}
	insertUntracked(trackedBubbles);
	
	
	//-----------------Recursion base case------------------------------------------
	if(iter == trackedBubbles->end()){
		return true;
	}
	//-----------------------------Next recusion step--------------------------------
	assignID(iter, _detector, _state, trackedBubbles);

};

//Update the new bubble positions(called by the detector)
void BubbleTracker::update(){
	if(_detector->bubbles.size()>1){
	int a =0;
	}
	map<unsigned int, Bubble>* trackedBubbles = _state->getCurrentState();
	map<unsigned int, Bubble>::iterator iter = trackedBubbles->begin();
	computeCloseIds(iter,trackedBubbles);
	assignID(iter, _detector, _state, trackedBubbles);
	//_detector->_capture->displayTrackedBubbles(_state);
	delete trackedBubbles;
};

//Prin the bubble positions to console
void BubbleTracker::printBubbles(){
	_state->printBubbles();
};


