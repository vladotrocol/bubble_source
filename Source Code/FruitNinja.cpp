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
	_generator(bg)
	{};

bool FruitNinja::init(){
	status = ST_READY;
	return true;
};

bool FruitNinja::start(){
	if(status==ST_READY){
		status=ST_PLAYING;
		pthread_create(&thread2,NULL,fwthreadFunction2,(void*)this);
	}
	return true;
};

char* getMat(int seed){
	if(seed%5==0){
		return "Examples/blue";
	}
	else if(seed%5==1){
		return "Examples/red";
	}
	else if(seed%5==2){
		return "Examples/green";
	}
	else if(seed%5==3){
		return "Examples/purple";
	}
	else if(seed%5==4){
		return "Examples/orange";
	}
}

void FruitNinja::run(){
	unsigned int ID = 5;
	char input;	
	cout<<"Press w for watermelon, o for orange:\n";
	while(status==ST_PLAYING){
		input = _getch();
		if(input == 'q'){
			createBubble(ID, "Examples/text1", _state, _generator, 'm');
			_generator->createPhysicalBubble('b');
		}
		else if(input == 'w'){
			createBubble(ID, "Examples/text2", _state, _generator, 'm');
			_generator->createPhysicalBubble('m');
		}
		else if(input == 'e'){
			createBubble(ID, "Examples/number1", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 'r'){
			createBubble(ID, "Examples/number2", _state, _generator, 'm');
			_generator->createPhysicalBubble('t');
		}
		else if(input == 't'){
			createBubble(ID, "Examples/test", _state, _generator, 'm');
			_generator->createPhysicalBubble('m');
		}
		else if(input == 'y'){
			createBubble(ID, "Examples/red", _state, _generator, 'm');
			_generator->createPhysicalBubble('m');
		}
		else if(input>48&&input<52){
			if(input>48){
				createBubble(ID, getMat(ID), _state, _generator, 's');
				ID++;
			}
			if(input>49){
				createBubble(ID,  getMat(ID), _state, _generator, 's');
				ID++;
			}
			if(input>50){
				createBubble(ID,  getMat(ID), _state, _generator, 's');
				ID++;
			}
				
			_generator->createPhysicalBubble(input);
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
	pthread_join(thread2,&result);
	return true;
};


void FruitNinja::createBubble(unsigned int ID, char* material, BubbleState* BS, BubbleGenerator* BG, char w){
	
	Bubble b;
	b.ID = ID;
	b.material = material;
	BS->addBubble(b);
	printf("bubbles created\n");
};