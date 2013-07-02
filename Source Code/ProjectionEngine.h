#ifndef _PROJECTION_ENGINE_C
#define _PROJECTION_ENGINE_C
#include "BubbleState.h"
#include "Calibrator.h"
class ProjectionEngine{
	public:
		BubbleState* _state;
		Calibrator* _calibrator;
		ProjectionEngine(BubbleState* BS, Calibrator *CA);
		void Draw();
};

#endif