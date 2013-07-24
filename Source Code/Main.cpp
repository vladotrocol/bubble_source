 
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

void createBubble(unsigned int ID, char* material, BubbleState* BS, BubbleGenerator* BG){
	Bubble b;
	b.ID = ID;
	b.material = material;
	if(BG->createPhysicalBubble()){
		BS->addBubble(b);
	}
	else{ 
		cerr<<"Bubble not generated";
	}
};

int main(){
	BubbleState& BS = BubbleState::instance();
	BubbleGenerator BG;
	BubbleTracker BT(&BS);
	OgreProjectionEngine ope;
	//FruitNinja app = new FruitNinja();
	unsigned int ID = 10;

	createBubble(ID, "Examples/watermelon",&BS, &BG);
	//createBubble(ID+1, "Examples/orange", &BS, &BG);
	
	BT.init();
	ope.init();
	BT.start();
	ope.start();

	cin.get();
	ope.stop();
	BT.stop();
	return 0;
};

//-------------Record code--------------
	//KOCVStream s;
	//s.record("_multipleBubbles.avi");
//------------------------------------------

//--------------Test kinect mode-----------
	//KOCVStream s;
	//while(1){
	//	s.readFrame();
	//	s.display("d");
	//	waitKey(1);
	//}
//--------------------------------------------
