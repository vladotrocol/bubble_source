#include "FruitNinja.h"
#include <conio.h>
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
	cout<<"Fruitty\n";
	while(status==ST_PLAYING){
		input = _getch();
		if(input == 'w'){
			cout<<"bubble\n";
			createBubble(ID, "Examples/watermelon", _state, _generator);
		}
		else if(input == 'o'){
			createBubble(ID, "Examples/orange", _state, _generator);
		}
		ID++;
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

void FruitNinja::createBubble(unsigned int ID, char* material, BubbleState* BS, BubbleGenerator* BG){
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