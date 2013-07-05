#include "Ogre.h"
#include "OgreProjectionEngine.h"
#include "./DisplayKernel/OgreApplications/BubbleApplication.h"



void* opeThreadFunction(void* a){
		OgreProjectionEngine* p=((OgreProjectionEngine*)a);
		p->run();
		return NULL;
};

bool OgreProjectionEngine::init(){
	ogreApp=new BubbleApplication();
	status = ST_READY;
	return true;
};

bool OgreProjectionEngine::start(){
	if(status==ST_READY){
		status=ST_PLAYING;
		pthread_create(&thread,NULL,opeThreadFunction,(void*)this);
	}
	return true;
};

void OgreProjectionEngine::run(){
	//Thread's main loop
	if(status==ST_PLAYING){
		ogreApp->go();
	}
	while(status==ST_PLAYING)
		Sleep(500); //Keep the thread alive.... WE DO NOT TERMINATE THE THREAD UNTIL SOMEONE CALLS OgreProjectionEngine::stop()
	//The status says we have to end
	Sleep(1000);//Keep it	
};


bool OgreProjectionEngine::stop(){
	if(status!=ST_PLAYING)
		return false;
	status=ST_READY;
	//await termination
	void* result;
	pthread_join(thread,&result);
	return true;
};
