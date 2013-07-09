#include "BubbleState.h"

//Constructors
BubbleState::BubbleState()
{;};

BubbleState::BubbleState(BubbleState& bs){
	trackedBubbles=bs.trackedBubbles;
	nonTrackedBubbles=bs.nonTrackedBubbles;
}

BubbleState& BubbleState::operator=(BubbleState& bs){
	BubbleState r;
	r.trackedBubbles=bs.trackedBubbles;
	r.nonTrackedBubbles=bs.nonTrackedBubbles;
	return r;
}

//Add a new bubble to the nonTracked queue
void BubbleState::addBubble(Bubble b){
	nonTrackedBubbles.push(b);
};

//Retrieve a nontracked bubble from the queue and start tracking it
unsigned int BubbleState::getUnknownBubble(void){
	if(nonTrackedBubbles.size()>0){
		Bubble b = this->nonTrackedBubbles.front();
		nonTrackedBubbles.pop();
		b.state = TRACKED;
		this->trackedBubbles[b.ID]=b;
		return b.ID;
	}
	else{
		return 0;
	}
};

void BubbleState::printBubbles(){
	map<unsigned int, Bubble>* bub = getCurrentState();
	for(map<unsigned int, Bubble>::iterator it = bub->begin(); it != bub->end(); ++it) {
		cout<<"ID: "<<it->second.ID<<" x: "<<it->second.center.x<<" y: "<<it->second.center.y<<" radius: "<<it->second.radius<<'\n';
	}
};

//Update the position of a bubble
void BubbleState::updateBubble(unsigned int ID, Point2f center, float radius){
	trackedBubbles[ID].center = center;
	trackedBubbles[ID].radius=radius;
	//cout<<ID<<" "<<center.x<<" "<<center.y<<" "<<radius<<'\n';
};

//Returns a copy of all tracked bubles
map<unsigned int, Bubble>* BubbleState::getCurrentState(void){
	return &trackedBubbles;
};

//Handles the event when a user pops a bubble
void BubbleState::notifyUserBreaks(unsigned int ID){
	trackedBubbles.erase(ID);
	cout<<"User popped bubble: "<<ID;
};

//Determines a tracking cycle (used for assuming if bubble broke)
void BubbleState::notifyTrackingFrame(void){

};

bool BubbleState::hasUnknownBubble(){
	return !nonTrackedBubbles.empty();
};