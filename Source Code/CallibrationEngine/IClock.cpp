#include "./CallibrationEngine/IClock.h"
#include "Ogre.h"

IClock* IClock::_instance=NULL;
static Ogre::Timer timer;

IClock& IClock::instance(){
		if(!_instance)
			_instance=new IClock();
		return *_instance;
	} 

void IClock::nextSecond(){
	timer.reset();
}

float IClock::getTimeMiliseconds(){
	static bool firstTime=true;
	
	if(firstTime){
		timer.reset();
		firstTime=false;
	}
	return ((float)(timer.getMicroseconds()+1)/1000);

}