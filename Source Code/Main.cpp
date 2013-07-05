 
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
#include "./DisplayKernel/OgreProjectionEngine.h"

using namespace std;

void createBubble(unsigned int ID, BubbleState* BS, BubbleGenerator* BG){
	Bubble b;
	b.ID = ID;
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
	unsigned int ID = 10;
	createBubble(ID, &BS, &BG);
	//createBubble(ID+1, &BS, &BG);
	BT.init();
	ope.init();
	BT.start();
	ope.start();
	cin.get();
	return 0;
};


/*int main(){
	BubbleApplication bubApp;
	bubApp.go();
	return 0;

}*/

