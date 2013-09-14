#include "BubbleState.h"

//Constructors
BubbleState::BubbleState()
{pthread_mutex_init(&lock,NULL);};

BubbleState::BubbleState(BubbleState& bs){
	trackedBubbles=bs.trackedBubbles;
	nonTrackedBubbles=bs.nonTrackedBubbles;
	pthread_mutex_init(&lock,NULL);
}

BubbleState& BubbleState::operator=(BubbleState& bs){
	BubbleState r;
	r.trackedBubbles=bs.trackedBubbles;
	r.nonTrackedBubbles=bs.nonTrackedBubbles;
	return r;
}

//Add a new bubble to the nonTracked queue
void BubbleState::addBubble(Bubble b){
	pthread_mutex_lock(&lock);
	nonTrackedBubbles.push(b);
	pthread_mutex_unlock(&lock);
};

//Retrieve a nontracked bubble from the queue and start tracking it
unsigned int BubbleState::getUnknownBubble(void){
	pthread_mutex_lock(&lock);
	if(nonTrackedBubbles.size()>0){
		Bubble b = this->nonTrackedBubbles.front();
		nonTrackedBubbles.pop();
		b.state = TRACKED;
		this->trackedBubbles[b.ID]=b;
		pthread_mutex_unlock(&lock);
		return b.ID;
	}
	else{
		pthread_mutex_unlock(&lock);
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
void BubbleState::updateBubble(unsigned int ID, Point3f center, float radius, bool tracked){
	pthread_mutex_lock(&lock);
	trackedBubbles[ID].write(center);
	trackedBubbles[ID].radius=radius;
	trackedBubbles[ID].state = tracked;
	pthread_mutex_unlock(&lock);
	//cout<<ID<<" "<<center.x<<" "<<center.y<<" "<<radius<<'\n';
};

//Returns a copy of all tracked bubles
map<unsigned int, Bubble>* BubbleState::getCurrentState(void){
	pthread_mutex_lock(&lock);
	map<unsigned int, Bubble>* result=new map<unsigned int, Bubble>();
	*result=trackedBubbles;
	pthread_mutex_unlock(&lock);
	return result;
};

//Handles the event when a user pops a bubble
void BubbleState::notifyUserBreaks(unsigned int ID){
	pthread_mutex_lock(&lock);
	trackedBubbles.erase(ID);
	pthread_mutex_unlock(&lock);
	cout<<"User popped bubble: "<<ID<<'\n';
};

void BubbleState::dontTrack(unsigned int ID){
	pthread_mutex_lock(&lock);
	trackedBubbles[ID].state = NON_TRACKED;
	pthread_mutex_unlock(&lock);
};

//Determines a tracking cycle (used for assuming if bubble broke)
void BubbleState::notifyTrackingFrame(void){

};

bool BubbleState::hasUnknownBubble(){
	return !nonTrackedBubbles.empty();
};