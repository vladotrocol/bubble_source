#ifndef _BUBBLE_GENERATOR
#define _BUBBLE_GENERATOR
#include "SerialCOM.h"
#include "COMToolkit.h"
class BubbleGenerator{
	public:
		Serial* arduino;
		BubbleGenerator(void);
		bool createPhysicalBubble(char b);	
};
#endif