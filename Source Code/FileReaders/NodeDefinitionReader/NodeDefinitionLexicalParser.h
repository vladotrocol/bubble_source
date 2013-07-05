#ifndef _NODE_DEFINITION_LEXICAL_PARSER
#define _NODE_DEFINITION_LEXICAL_PARSER

#include "./FileReaders/NodeDefinitionReader/LexicalCathegories.h"
#include "./FileReaders/LexicalParser.h"
#include "./FileReaders/FileInputStream.h"
#include "./BasicElements/OID.h"


class NodeDefinitionLexicalParser: public LexicalParser{
	Token curToken;
	int status;
	static const int INIT=0;
	static const int READY=1;
	static const int END=2;
public:
	NodeDefinitionLexicalParser(std::string file):curToken(NodeDefinitionLexicalCathegories::INVALID){
		input=new FileInputStream(file);
		status=INIT;
	}

	virtual Token getToken(){
		if(status==INIT){
			status=NodeDefinitionLexicalParser::READY;
			this->nextToken();			
		}
		return curToken;
	}

	virtual bool nextToken(){
		int q=0;
		//Variables para contener los calores asociados a cada Token;
		int value;
		int * v;
		OID oid;
		OID*o;
		char str[256];
		int strIndex;
		char* s;
		
		if(status==NodeDefinitionLexicalParser::END)
			return false;

		status=NodeDefinitionLexicalParser::READY;
		
		while(true){
			switch(q){
				case -1:
					//This is an error. The input is skipped untill an ' ','\n' or '\t' is found:
					switch(input->read()){
						case ' ':
						case '\n':
						case '\t':
						case 13:
							q=0;
							if(!input->next())status=NodeDefinitionLexicalParser::END;
							curToken=Token(NodeDefinitionLexicalCathegories::INVALID);
							return true;
						default:
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;					
								return false;
							}
					}
					break;
				case 0:
					
					switch(input->read()){
						case '(':
							if(!input->next())
								status=NodeDefinitionLexicalParser::END;
							curToken=Token(NodeDefinitionLexicalCathegories::ABRE_PAR);
							return true;
							break;
						case ')':
							if(!input->next())
								status=NodeDefinitionLexicalParser::END;
							curToken=Token(NodeDefinitionLexicalCathegories::CIERRA_PAR);
							return true;
							break;
						case ';':
							if(!input->next())
								status=NodeDefinitionLexicalParser::END;
							curToken=Token(NodeDefinitionLexicalCathegories::PYC);
							return true;
							break;
						case '/':
							if(!input->next())
								status=NodeDefinitionLexicalParser::END;
							curToken=Token(NodeDefinitionLexicalCathegories::BARRA);
							return true;
							break;
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							q=5;
							value=0;
							value=(int)(input->read()-'0');
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case '\"':
							strIndex=0;
							q=13;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case 'N':
							q=15;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case 'I':
							q=23;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case 'U':
							q=32;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;

						case 'C':
							q=41;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case '[':
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
							}
							curToken=Token(NodeDefinitionLexicalCathegories::ABRE_COR);
							return true;
							break;
						case ']':
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
							}
							curToken=Token(NodeDefinitionLexicalCathegories::CIERRA_COR);
							return true;
							break;
						case ' ':
						case '\n':
						case '\t':
							q=0;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;
							break;
                    }

					break;

				case 5:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=value*10+((int)input->read()-'0');
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case '.':
							//IT IS AN OID!!
							oid[0]=value;
							value=0;
							q=7;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							//IT IS AN INTEGER!!
							curToken=Token(NodeDefinitionLexicalCathegories::ENTERO);
							v=new int;
							*v=value;
							curToken.values["value"]=v;
							return true;

					}
					break;
				case 7:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=(int)(input->read()-'0');
							q=8;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;
							break;					
					}
					break;
				case 8:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=10*value+(int)(input->read()-'0');
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case '.':
							//second member of the OID read.
							oid[1]=value;
							value=0;
							q=9;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;					
							break;
					}
					break;
				case 9:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=(int)(input->read()-'0');
							q=10;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;
							break;					
					}
					break;
				case 10:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=10*value+(int)(input->read()-'0');
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						case '.':
							//third member of the OID read.
							oid[2]=value;
							value=0;
							q=11;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;					
							break;
					}
					break;

				case 11:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=(int)(input->read()-'0');
							q=12;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						default:
							q=-1;
							break;					
					}
					break;
				case 12:
					switch(input->read()){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value=10*value+(int)(input->read()-'0');
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						
						default:
							//return the OID recognized:
							oid[3]=value;
							curToken=Token(NodeDefinitionLexicalCathegories::TOKEN_OID);
							o=new OID();
							*o=oid;
							curToken.values["OID"]=o;
							return true;			
							break;
					}
					break;
	
				case 13:
					//Parsing a string:
					switch(input->read()){
						case '\"':
							q=0;
							curToken=Token(NodeDefinitionLexicalCathegories::STRING);
							str[strIndex]='\0';
							s= new char[strIndex+1];
							for(int i=0;i<=strIndex;i++)
								s[i]=str[i];
							curToken.values["string"]=s;							
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							return true;
							break;
						case '\n':
							q=-1;
							break;
						default: 
							str[strIndex]=input->read();
							strIndex++;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
					}
					break;
				case 15:
					if(input->read()=='o'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 16:
					if(input->read()=='d'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 17:
					if(input->read()=='e'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;

				case 18:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
						case '.':
						case ';':
						case '/':
						case '(':
						case ')':
						case '[':
						case ']'://Node
							curToken=Token(NodeDefinitionLexicalCathegories::NODE);
							return true;

						case 'D'://NodeData
							q++;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
						
						default:
							q=-1;
							if(!input->next()){
								status=NodeDefinitionLexicalParser::END;
								return false;
							}
							break;
					}
					break;
				case 19:
					if(input->read()=='a'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 20:
					if(input->read()=='t'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;				
				case 21:
					if(input->read()=='a'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 22:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
						case '.':
						case ';':
						case '/':
						case '(':
						case ')':
						case '[':
						case ']':
							curToken=Token(NodeDefinitionLexicalCathegories::NODE_DATA);
							return true;
						default:
							//Palabra rara
							q=-1;
							break;

					}

					break;

				case 23:
					if(input->read()=='n'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 24:
					if(input->read()=='p'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 25:
					if(input->read()=='u'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 26:
					if(input->read()=='t'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 27:
					if(input->read()=='P'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 28:
					if(input->read()=='o'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 29:
					if(input->read()=='r'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;

				case 30:
					if(input->read()=='t'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
									
				case 31:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
						case '.':
						case ';':
						case '/':
						case '(':
						case ')':
						case '[':
						case ']':
							curToken=Token(NodeDefinitionLexicalCathegories::INPUT_PORT);
							return true;
						default:
							//Palabra rara
							q=-1;
							break;

					}

					break;
														
				case 32:
					if(input->read()=='p'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 33:
					if(input->read()=='p'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 34:
					if(input->read()=='e'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 35:
					if(input->read()=='r'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 36:
					if(input->read()=='N'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 37:
					if(input->read()=='o'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 38:
					if(input->read()=='d'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 39:
					if(input->read()=='e'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 40:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
						case '.':
						case ';':
						case '/':
						case '(':
						case ')':
						case '[':
						case ']':
							curToken=Token(NodeDefinitionLexicalCathegories::UPPER_NODE);
							return true;
						default:
							//Palabra rara
							q=-1;
							break;

					}

					break;

				case 41:
					if(input->read()=='h'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 42:
					if(input->read()=='i'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 43:
					if(input->read()=='l'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 44:
					if(input->read()=='d'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 45:
					if(input->read()=='r'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 46:
					if(input->read()=='e'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 47:
					if(input->read()=='n'){
						q++;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}else {
						q=-1;
						if(!input->next()){
							status=NodeDefinitionLexicalParser::END;
							return false;
						}
					}
					break;
				case 48:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
						case '.':
						case ';':
						case '/':
						case '(':
						case ')':
						case '[':
						case ']':
							curToken=Token(NodeDefinitionLexicalCathegories::CHILDREN);
							return true;
						default:
							//Palabra rara
							q=-1;
							break;

					}
					break;
				default:
					return false;
			
			}
		
		}

	}


};

#endif

