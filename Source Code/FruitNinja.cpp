#include "FruitNinja.h"

//Helper function to create the thread
void* fwthreadFunction(void* a);


void* fwthreadFunction2(void* a){
		FruitNinja* p=((FruitNinja*)a);
		p->run();
		return NULL;
};

FruitNinja::FruitNinja(BubbleState* bs, BubbleGenerator* bg):
	_state(bs),
	_generator(bg){};

bool FruitNinja::init(){return true;};

bool FruitNinja::start(){
	if(status==ST_READY){
		status=ST_PLAYING;
		pthread_create(&thread,NULL,fwthreadFunction2,(void*)this);
	}
	return true;
};

void FruitNinja::run(){
	unsigned int ID = 1;
	char input;
	while(cin >> input && status==ST_PLAYING){
		cin.clear();
		if(input == 'w'){
			createBubble(ID, "Examples/watermelon");
		}
		else if(input == 'o'){
			createBubble(ID+1, "Examples/orange");
		}
	}
};

bool FruitNinja::stop(){
	if(status!=ST_PLAYING)
		return false;
	status=ST_READY;
	//await termination
	void* result;
	pthread_join(thread,&result);
	return true;
};

void FruitNinja::createBubble(unsigned int ID, char* material){
	Bubble b;
	b.ID = ID;
	b.material = material;
	if(_generator->createPhysicalBubble()){
		_state->addBubble(b);
	}
	else{ 
		cerr<<"Bubble not generated";
	}
};