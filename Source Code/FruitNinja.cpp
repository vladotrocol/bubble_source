#include "FruitNinja.h"
#include <conio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


//This is where you set the correct sphere size depending on the size of the physical bubble
float getSphereSize(char size){
	switch(size){
	case 's':
		return 35.0f;
	case 'b': 
		return 50.0f;
	case 'm':
		return 42.0f;
	}
	return -1.0f;
}

void FruitNinja::startStudy(){ 
    // read file line by line
	std::ifstream input("study1.txt"); //input file
	ofstream result;
	
	//-------------------generating the time stamp----
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [50];
		 time ( &rawtime );
		 timeinfo = localtime ( &rawtime );
		 sprintf_s(buffer, "results-%d-%d - %d-%02d-%02d.log", 
			 (timeinfo->tm_mon),(timeinfo->tm_mday),
			 (timeinfo->tm_hour),(timeinfo->tm_min),
			 (timeinfo->tm_sec));
		 string filename = buffer;
	//--------------------------------------

	result.open (filename); //output file



	int nr=0;      //current bubble # which is alsu tracked bubble ID
	char correct = 0;  //current answer
	char size=0;  //current bubble size (s/b/m)
	string material; //current bubble texture (not png.. but ogre material in the example.material file
	//--------Wait for ogre prints to finish---------
	char a=0;
	a=_getche();
	//------------------------------------------------


	printf("\nStart trial\n");
	
	//iterrate through each line
    for (std::string line; std::getline(input, line); ) {
		
		//split the line into tokens (tab delimiter)
		std::vector<std::string> tokens = split(line, '\t'); 


		nr = std::stoi(tokens[0]); //bubble #
		
		material = tokens[1]; //bubble texture

		size = tokens[2][0]; //bubble size (s/b/m chars)
		
		

		//print bubble info
		printf("\nBubble %d: size %c; texture: %s;\n", nr, size,  ("Examples/"+material).c_str());
		
		//create virtual bubble (ogre sphere)
		createBubble(nr, getSphereSize(size), "Examples/"+material, _state, _generator, 'm');
		//create(physical bubble(machine)
		_generator->createPhysicalBubble(size);

		//check for answer
		printf("Correct? (y/n/r)\n");
		correct = _getche();

		//if answer is r keep generating physical bubbles
		//while traying to track the same id
		while(correct=='r'){
			printf("\nRepeated a bubble\n");
			_generator->createPhysicalBubble(size);
			printf("Correct? (y/n/r)\n");
			correct = _getche();
		}

		//output to file
		result<<nr<<"\t"<<material<<" \t"<<size<<"\t"<<correct<<"\n";
    }

	//close files
	printf("\nFinished trial. You can close the program now..\n");
	result.close();
	input.close();
}

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

string getMat(int seed){
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

	startStudy();
	/*char input;	
	cout<<"Press w for watermelon, o for orange:\n";
	while(status==ST_PLAYING){
		input = _getch();
		if(input == 'q'){
			createBubble(ID, 35.0, "Examples/text1", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 'w'){
			createBubble(ID, 35.0, "Examples/text2", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 'e'){
			createBubble(ID, 35.0, "Examples/text3", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 'r'){
			createBubble(ID, 30.0, "Examples/number1", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 't'){
			createBubble(ID, 30.0, "Examples/test", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input == 'y'){
			createBubble(ID, 30.0, "Examples/red", _state, _generator, 'm');
			_generator->createPhysicalBubble('s');
		}
		else if(input>48&&input<52){
			if(input>48){
				createBubble(ID, 30.0, getMat(ID), _state, _generator, 's');
				ID++;
			}
			if(input>49){
				createBubble(ID, 30.0,  getMat(ID), _state, _generator, 's');
				ID++;
			}
			if(input>50){
				createBubble(ID, 30.0,  getMat(ID), _state, _generator, 's');
				ID++;
			}
				
			_generator->createPhysicalBubble(input);
		}
		ID++;
	}*/
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


void FruitNinja::createBubble(unsigned int ID,float radius,string material, BubbleState* BS, BubbleGenerator* BG, char w){
	
	Bubble b;
	b.ID = ID;
	b.setRadius = radius;
	b.material = material;
	BS->addBubble(b);
	printf("bubbles created\n");
};