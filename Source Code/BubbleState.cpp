#include "BubbleState.h"

//Constructor
BubbleState::BubbleState()
{;};

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

//Update the position of a bubble
void BubbleState::updateBubble(unsigned int ID, float x, float y, float radius){
	trackedBubbles[ID].center = Point2f(x,y);
};

//Returns a copy of all tracked bubles
map<unsigned int, Bubble> BubbleState::getCurrentState(void){
	map<unsigned int, Bubble> trackedBubblesCopy(trackedBubbles);
	return trackedBubblesCopy;
};

//Handles the event when a user pops a bubble
void BubbleState::notifyUserBreaks(unsigned int ID){
	trackedBubbles.erase(ID);
	cout<<"User popped bubble: "<<ID;
};

//Determines a tracking cycle (used for assuming if bubble broke)
void BubbleState::notifyTrackingFrame(void){

};