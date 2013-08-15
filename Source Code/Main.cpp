 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h> //DIRTY dirty trick. This needs to be included first than anything else. Otherwise UDP does not work
#include "windows.h"
#endif

#include "./DisplayKernel/OgreApplications/BubbleApplication.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include "BubbleTracker.h"
#include "BubbleState.h"
#include "BubbleGenerator.h"
#include "FruitNinja.h"
#include "./DisplayKernel/OgreProjectionEngine.h"

using namespace std;

int main(){
	bool test = false;
	bool record = false;

	//Record
	if(record){
		KOCVStream s;
		s.record("_b2.avi");
	}
	//or test kinect
	else if(test){
		KOCVStream s;
		while(1){
			s.readFrame();
			s.display("d");
			s._stream->release();
			waitKey(1);
		}
	}
	//Normal mode
	else{
		BubbleState& BS = BubbleState::instance();
		BubbleGenerator* BG = new BubbleGenerator();
		BubbleTracker BT(&BS);
		OgreProjectionEngine ope;
		FruitNinja app(&BS, BG);

		//createBubble(ID, "Examples/watermelon",&BS, &BG);
		//createBubble(ID+1, "Examples/orange", &BS, &BG);
		//createBubble(ID+12, "Examples/orange", &BS, &BG);
		//createBubble(ID+13, "Examples/orange", &BS, &BG);
		
		BT.init();
		app.init();
		ope.init();
		BT.start();
		ope.start();
		app.start();

		while(1){
		}
		ope.stop();
		BT.stop();
		app.stop();
	}
	return 0;
};
