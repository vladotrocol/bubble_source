#ifndef _BUBBLE_TRACKER_C
#define _BUBBLE_TRACKER_C
#include "IBubbleTracker.h"
#include "BubbleDetector.h"
#include "BubbleState.h"
#include "Calibrator.h"

#define minBubbleSize 10
#define maxBubbleSize 100
#define travelMax 20

class BubbleTracker: public IBubbleTracker{
	public:
		BubbleDetector* _detector;
		BubbleState* _state;
		Calibrator* _calibrator;
		vector<Bubble> bubbles;
		vector<Bubble> prevBubbles;
		bool init(void);
		bool start(void);
		bool stop(void);
		void update(float x, float y, float radius);
		vector<pair<double, Point>> findNearestBubbles();
		int guessBubblePoppedandBorn(vector<pair<double, Point>> bubbleComps);
		vector<Bubble> IDBubbles();
		BubbleTracker(BubbleState* BS);
};
#endif