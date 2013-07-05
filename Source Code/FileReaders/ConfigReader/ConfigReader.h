#ifndef _CONFIG_READER
#define _CONFIG_READER

#include "./FileReaders/SemanticalParser.h"
#include "./FileReaders/GenericLexicalParser.h"
//#include "./CommunicationElements/CommunicationLayer.h"

class Pair{
public:
	std::string attribute;
	std::string value;
};



class ConfigReader:public SemanticalParser{
Token curToken;
public:
	std::map<int,Pair> config; 
	ConfigReader():curToken(-1){
		this->input=NULL;
	}
	
	ConfigReader(InputStream* input):curToken(-1){
		this->input=new GenericLexicalParser(input);
	}

	~ConfigReader(){
		if(input)delete input;
	}
	//Parses a configurationFile
	//<S>-> <ReadLine><S>
	virtual bool parse(){		
		if(!input)
			return false;
		std::string attribute,value;
		int lineNumber;
		bool result= false;
		while(readLine(&lineNumber,&attribute,&value)){
			Pair p;
			p.attribute=attribute;
			p.value=value;
			config[lineNumber]=p;
			result=true;
		}
		return result;
	}

	ConfigReader& operator=(ConfigReader r){
		while(this->config.begin()!=this->config.end())
			this->config.erase(this->config.begin());
		std::map<int,Pair>::iterator itR;

		for(itR=r.config.begin();itR!=r.config.end();itR++)
			this->config[itR->first]=itR->second;
		return *this;
	}

protected:
	//<ReadLine> -> Int String String
	bool readLine(int* lineNumber,std::string* attribute, std::string* value){
		curToken=input->getToken();
		if(curToken.tokenId!=GenericLexicalCathegories::T_ENTERO)return false;
		*lineNumber=*((int*)curToken.values["value"]);
		delete (int*)curToken.values["value"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=GenericLexicalCathegories::T_STRING)return false;
		*attribute=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();
	
		curToken=input->getToken();
		if(curToken.tokenId!=GenericLexicalCathegories::T_STRING)return false;
		*value=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();

		return true;
	}


};

#endif
