#ifndef _GENERIC_CONFIG_READER
#define _GENERIC_CONFIG_READER

#include "./FileReaders/SemanticalParser.h"
#include "./FileReaders/GenericLexicalParser.h"
#include "./FileReaders/GenericConfigReader/FieldData.h"
#include "./BasicElements/Operation.h"

class GenericConfigReader: public SemanticalParser{

	Token curToken;
public:
	FieldData configuration;
	GenericConfigReader():curToken(-1){
		this->input=NULL;
	}
	
	GenericConfigReader(InputStream* input):curToken(-1){
		this->input=new GenericLexicalParser(input);
	}

	~GenericConfigReader(){
		if(input)delete input;
	}

	bool parse(){
		if(!input)
			return false;
		std::string attribute,value;
		return Node(&this->configuration);
		
	}
protected:
	/* Describes a valid File. 	
	<File> -> <Attribute> <File>*/
	
	bool File(FieldData* f){
		FieldData* subfield=new FieldData();
		while(Attribute(subfield)){
			f->addSubfield(subfield);
			subfield=new FieldData();
		}
		delete subfield;
		//check 'siguientes': If the next token is '{' or 'eof', the parsing is correct.
		return true;
	}

	//<Attribute> -> T_RESERVED_WORD <Value> <Children>
	bool Attribute(FieldData* f){
		//Parse T_RESERVED_WORD
		std::string fieldName,fieldValue;
		curToken=input->getToken();
		if(curToken.tokenId!=GenericLexicalCathegories::T_RESERVED_WORD)return false;
		fieldName=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		//Parse <Value>
		if(!Value(&fieldValue))
			return false;
        		
		//Parse <Children>
		if(!Children(f))
			return false;
		
		//Set the data:
		f->setParameters(fieldName,fieldValue);
		return true;
	}

	//<Value> -> T_ENTERO|T_REAL|T_OID|<Matrix>|T_RESERVED_WORD|T_STRING| lambda
	bool Value(std::string* fieldValue){
		curToken=input->getToken();
		//Process if it is not 'lambda'
		switch(curToken.tokenId){
			case T_ENTERO:
				{
					int* pInt=(int*)curToken.values["value"];
					*fieldValue=Operation::fromIntToStr(*pInt);
					delete pInt;
					input->nextToken();
					return true;
				}
				break;
			case T_REAL:
				{
					float* pReal=(float*)curToken.values["value"];
					*fieldValue=Operation::fromFloatToStr(*pReal);
					delete pReal;
					input->nextToken();
					return true;
				}
				break;
			case T_OID:
				{
					OID* pOID=(OID*)curToken.values["OID"];
					*fieldValue=Operation::fromOIDToStr(*pOID);
					delete pOID;
					input->nextToken();
					return true;
				}
				break;
			case T_RESERVED_WORD:
				{
					char* pChar=(char*)curToken.values["string"];
					*fieldValue=std::string(pChar);
					delete pChar;
					input->nextToken();
					return true;
				}
				break;
			case T_STRING:
				{
					char* pChar=(char*)curToken.values["string"];
					*fieldValue=std::string(pChar);
					delete pChar;
					input->nextToken();
					return true;
				}
				break;
			case T_ABRE_COR:
				{
					Ogre::Matrix4 m;
					if(Matrix(&m)){
						*fieldValue=Operation::fromMatrixToStr(m);
						return true;
					}
					return false;
				}
				break;
			case T_ABRE_LLA:
			case T_PYC:
				// <Value> -> lambda
				*fieldValue="";
				return true;
			default:
				return false;
		}
	}

	// <Children> -> T_PYC | T_ABRE_LLA <Node> T_CIERRA_LLA
	bool Children(FieldData* f){
		curToken=input->getToken();
		//Process if it is not 'lambda'
		switch(curToken.tokenId){
			case T_PYC:
				// <Children> -> T_PYC
				input->nextToken();
				return true;
			case T_ABRE_LLA:
				// <Children> -> T_ABRE_LLA <Node> T_CIERRA_LLA
				{
					curToken=input->getToken();
					if(curToken.tokenId!=GenericLexicalCathegories::T_ABRE_LLA)return false;
					input->nextToken();

					if(!Node(f))return false;

					curToken=input->getToken();
					if(curToken.tokenId!=GenericLexicalCathegories::T_CIERRA_LLA)return false;
					input->nextToken();

					return true;
				}
			default:
				return false;


		}
	}

	bool Node(FieldData* f){
		FieldData* subfield=new FieldData();
		while(Attribute(subfield)){
			f->addSubfield(subfield);
			subfield=new FieldData();
		}
		delete subfield;
		//check 'siguientes': If the next token is '}', the parsing is correct.
		curToken=input->getToken();
		return curToken.tokenId==T_CIERRA_LLA || curToken.tokenId==T_PYC;
	}

	//<Matrix>--> [<Number>; <Number>;...<Number>;]
	bool Matrix(Ogre::Matrix4* pos){
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		float value_i_j;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				if(!Number(&value_i_j))
					return false;
				curToken=input->getToken();
				if(i==3 && j==3)
					;
				else if (curToken.tokenId==T_COMA)
					input->nextToken();
				else 
					return false;
				(*pos)[i][j]=value_i_j;
			}

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();
				
		return true;
	}

	bool Number(float* f){
		curToken=input->getToken();
		if(curToken.tokenId==T_ENTERO){
			*f=(*(int*)curToken.values["value"])*1.0f;
			delete 	(int*)curToken.values["value"];
			input->nextToken();
			return true;
		}
		else if(curToken.tokenId==T_REAL){
			*f=(*(float*)curToken.values["value"])*1.0f;
			delete 	(float*)curToken.values["value"];
			input->nextToken();
			return true;
		}else return false;	
	}

	
};

#endif