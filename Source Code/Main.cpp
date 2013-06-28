 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h> //DIRTY dirty trick. This needs to be included first than anything else. Otherwise UDP does not work
#include "windows.h"
#endif


#include "./DisplayKernel/OgreApplications/CompositeOgreApplication.h"
#include "./DisplayKernel/OgreApplications/LayerSimulation.h"
#include "./DisplayKernel/OgreApplications/VideoPlayer.h"
#include "./WaterDisplayUSBController/XMOSControlPlugin.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include "BubbleTracker.h"
#include "BubbleState.h"
#include "BubbleGenerator.h"

using namespace std;

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
	return 0;
}

void createBubble(BubbleState* BS, BubbleGenerator* BG){
	Bubble b(10);
	if(BG->createPhysicalBubble()){
		BS->addBubble(b);
	}
};