#ifndef _BUBBLE_STATE_C
#define _BUBBLE_STATE_C

#include <iostream>
#include <map>
#include <queue>
#include <pthread.h>
#include "Bubble.h"

using namespace std;

class IBubbleState{
	public:
		virtual void addBubble(Bubble b)=0;
		virtual unsigned int getUnknownBubble(void)=0;
		virtual void updateBubble(unsigned int ID, Point2f center, float radius)=0;
		virtual map<unsigned int, Bubble>* getCurrentState(void)=0;
		virtual void notifyUserBreaks(unsigned int ID)=0;
		virtual void notifyTrackingFrame(void)=0;
};

class BubbleState: public IBubbleState{
	private:

		queue<Bubble> nonTrackedBubbles;
		map<unsigned int, Bubble> trackedBubbles;
		pthread_mutex_t lock;

		//--------------------Constructor----------------------
		BubbleState(void);
		BubbleState(BubbleState& b);
		BubbleState& operator=(BubbleState& b);

	public:

		//----------------------Methods------------------------

		//Fake Contructor (Restricts to only one instance)
		static BubbleState& instance(){
			static BubbleState _instance;
			return _instance;
		};

		void printBubbles();

		bool hasUnknownBubble();

		//Add a new bubble to the nonTracked queue
		void addBubble(Bubble b);

		//Retrieve a nontracked bubble from the queue and start tracking it
		unsigned int getUnknownBubble(void);

		//Update the position of a bubble
		void updateBubble(unsigned int ID, Point2f center, float radius);

		//Returns a copy of all tracked bubles
		map<unsigned int, Bubble>* getCurrentState(void);

		//Handles the event when a user pops a bubble
		void notifyUserBreaks(unsigned int ID);

		//Determines a tracking cycle (used for assuming if bubble broke)
		void notifyTrackingFrame(void);
};

#endif