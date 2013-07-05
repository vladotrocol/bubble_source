#ifndef _BASIC_TYPES_PARSER
#define _BASIC_TYPES_PARSER

#include "./FileReaders/SemanticalParser.h"
#include "./FileReaders/GenericLexicalParser.h"
#include "./FileReaders/BufferInputStream.h"
using namespace GenericLexicalCathegories;
class BasicTypesReader{

public:
	static char* writeMatrix(Ogre::Matrix4 m){
		std::string result="";

		char buffer[1024];
		
		sprintf(buffer,"[");
		result.append(buffer);

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				sprintf(buffer,"%10.4f,",m[i][j]);
				result.append(buffer);
			}
		char* res= new char[result.size()+1];
		strcpy(res,result.c_str());
		res[result.size()-1]=']';
		res[result.size()]='\0';
		return res;

	
	}

	static char* writeFloat(float f){
		std::string result="";

		char buffer[1024];
		sprintf(buffer,"%10f",f);
		result.append(buffer);

		char* res=new char[result.size()+1];
		strcpy(res,result.c_str());
		res[result.size()]='\0';

		return res;
	}

	static char* writeInteger(int i){
		std::string result="";

		char buffer[1024];
		sprintf(buffer,"%d",i);
		result.append(buffer);

		char* res=new char[result.size()+1];
		strcpy(res,result.c_str());
		res[result.size()]='\0';

		return res;

	}

	static char* writeString(std::string str){
		std::string result="";

		char buffer[1024];
		sprintf(buffer,"\"%s\"",str.c_str());
		result.append(buffer);

		char* res=new char[result.size()+1];
		strcpy(res,result.c_str());
		res[result.size()]='\0';

		return res;

	}
	
	
	//THESE METHODS WASTE MEMORY, AS THEY DO NOT DE-ALLOCATE THE GenericLexicalParser OR THE BufferInputStream 
	// ALLOCATED...
	static bool Matrix (std::string str,Ogre::Matrix4* result){
		char* buffer=(char*)str.c_str();
		BufferInputStream* is=new BufferInputStream(buffer);
		GenericLexicalParser* lp=new GenericLexicalParser(is);
		bool b= BasicTypesReader::Matrix(lp,result);
		delete lp;
		return b;
	}
	static bool Float (std::string str,float* result){
		char* buffer=(char*)str.c_str();
		BufferInputStream* is=new BufferInputStream(buffer);
		GenericLexicalParser* lp=new GenericLexicalParser(is);
		bool b= BasicTypesReader::Float(lp,result);
		delete lp;
		return b;
		
	}
	static bool Integer (std::string str,int* result){
		char* buffer=(char*)str.c_str();
		BufferInputStream* is=new BufferInputStream(buffer);
		GenericLexicalParser* lp=new GenericLexicalParser(is);
		bool b= BasicTypesReader::Integer(lp,result);
		delete lp;
		return b;
	}

	static bool String(std::string str, std::string* result){
		char* buffer=(char*)str.c_str();
		BufferInputStream* is=new BufferInputStream(buffer);
		GenericLexicalParser* lp=new GenericLexicalParser(is);
		bool b= BasicTypesReader::String(lp,result);
		delete lp;
		return b;
	}

	//THESE METHODS DO NOT WASTE ANYTHING, BUT THEY ARE MORE DIFFICULT TO USE.
	static bool Matrix(LexicalParser *input,Ogre::Matrix4* result){
		Token curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		float value_i_j;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				if(!Number(input,&value_i_j))
					return false;

				if((i!=3 || j!=3)){
					curToken=input->getToken();
					if(curToken.tokenId!=T_COMA)return false;
					input->nextToken();
				}

				(*result)[i][j]=value_i_j;
			}

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();
				
		return true;
	}

	static bool Float(LexicalParser* input,float* value){
		Token curToken=input->getToken();
		if(curToken.tokenId==T_REAL){
			*value=(*(float*)curToken.values["value"])*1.0f;
			delete 	(float*)curToken.values["value"];
			input->nextToken();
			return true;
		}else if(curToken.tokenId==T_ENTERO){
			*value=(*(int*)curToken.values["value"])*1.0f;
			delete 	(int*)curToken.values["value"];
			input->nextToken();
			return true;
		}
		
		return false;	
	}

	static bool Integer(LexicalParser* input,int* value){
		Token curToken=input->getToken();
		if(curToken.tokenId==T_ENTERO){
			*value=(*(int*)curToken.values["value"]);
			delete 	(int*)curToken.values["value"];
			input->nextToken();
			return true;
		}
		return false;
	}

	static bool String(LexicalParser* input,std::string* result){
		Token curToken=input->getToken();
		if(curToken.tokenId==T_STRING){
			*result= std::string((char*)curToken.values["string"]);
			delete (char*)curToken.values["string"];
			input->nextToken();
			return true;
		}
		return false;
	}




private:
	static bool Number(LexicalParser* input,float* value){
		Token curToken=input->getToken();
		if(curToken.tokenId==T_ENTERO){
			*value=(*(int*)curToken.values["value"])*1.0f;
			delete 	(int*)curToken.values["value"];
			input->nextToken();
			return true;
		}
		else if(curToken.tokenId==T_REAL){
			*value=(*(float*)curToken.values["value"])*1.0f;
			delete 	(float*)curToken.values["value"];
			input->nextToken();
			return true;
		}else return false;	
	}



};
#endif

