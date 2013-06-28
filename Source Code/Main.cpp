 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h> //DIRTY dirty trick. This needs to be included first than anything else. Otherwise UDP does not work
#include "windows.h"
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include "BubbleTracker.h"
#include "BubbleState.h"
#include "BubbleGenerator.h"

using namespace std;

void createBubble(BubbleState* BS, BubbleGenerator* BG){
	Bubble b;
	if(BG->createPhysicalBubble()){
		BS->addBubble(b);
	}
};

int main(){
//	Kinect K;
//	Filters F;
//	cout<<K.initialiseKinect()<<'\n';
//	KOCVStream STREAM(K,F);
//	while(1){
//	STREAM.display("dei");
//	}

	BubbleState BS = BubbleState::instance();
	BubbleGenerator BG;
	BubbleTracker BT(&BS);
	BT.init();
	BT.start();
	createBubble(&BS, &BG);
	return 0;
};