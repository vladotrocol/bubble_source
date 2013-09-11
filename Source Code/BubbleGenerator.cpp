#include "BubbleGenerator.h"

BubbleGenerator::BubbleGenerator(){
	COMToolkit::connect();
};

bool BubbleGenerator::createPhysicalBubble(char b){
	COMToolkit::sendByte(b);
	return true;
};