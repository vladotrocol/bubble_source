#ifndef _BUBBLE_TRACKER_C
#define _BUBBLE_TRACKER_C
#include "IBubbleTracker.h"
#include "BubbleDetector.h"
#include "BubbleState.h"
#include "Calibrator.h"

class BubbleTracker: public IBubbleTracker{
	public:
		BubbleDetector* _detector;
		BubbleState* _state;
		Calibrator* _calibrator;
		bool init(void);
		bool start(void);
		bool stop(void);
		void update(float x, float y, float radius);
		BubbleTracker(BubbleState* BS);
};
#endif