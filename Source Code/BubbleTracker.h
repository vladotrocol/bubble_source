#ifndef _BUBBLE_TRACKER_C
#define _BUBBLE_TRACKER_C
#include "IBubbleTracker.h"
#include "BubbleDetector.h"
#include "BubbleState.h"
#include "Calibrator.h"
#include "ProjectionEngine.h"

#define travelMax 20

class BubbleTracker: public IBubbleTracker{
	public:
		BubbleDetector* _detector;
		BubbleState* _state;
		Calibrator* _calibrator;
		ProjectionEngine* _projector;
		bool init(void);
		bool start(void);
		bool stop(void);
		void update();
		void printBubbles();
		BubbleTracker(BubbleState* BS);
		void updateFPS(bool newFrame);
		void insertUntracked(map<unsigned int, Bubble>* trackedBubbles);
		void computeCloseIds(map<unsigned int, Bubble>::iterator iter,map<unsigned int, Bubble>* trackedBubbles);
		bool assignID(map<unsigned int, Bubble>::iterator iter,
				BubbleDetector* _detector,
				BubbleState* _state, 
				map<unsigned int, Bubble>* trackedBubbles);
};

#endif