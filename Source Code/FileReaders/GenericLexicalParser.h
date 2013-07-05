#ifndef _GENERIC_LEXICAL_PARSER
#define _GENERIC_LEXICAL_PARSER

#include "./FileReaders/GenericLexicalCathegories.h"
#include "./FileReaders/LexicalParser.h"
#include "./FileReaders/FileInputStream.h"
#include "./BasicElements/OID.h"
#include "./BasicElements/BasicTypes.h"


class GenericLexicalParser: public LexicalParser{
	Token curToken;
	int status;
	static const int INIT=0;
	static const int READY=1;
	static const int END=2;
	InputStream* input;
public:
	GenericLexicalParser(InputStream* input) :curToken(GenericLexicalCathegories::T_INVALID){
		status=GenericLexicalParser::INIT;
		this->input=input;
	}

	~GenericLexicalParser(){
		if(input)delete input;
	}

	virtual Token getToken(){
		if(status==GenericLexicalParser::INIT){
			this->nextToken();			
		}
		return curToken;
	}

	virtual bool nextToken(){
		int q=0;
		//Variables para contener los calores asociados a cada Token;
		int value;
		int * v;
		float realValue;
		float weight;
		float * f;
		OID oid;
		OID*o;
		char str[10256];
		int strIndex;
		char* s;
		int numberSign=1;
		
		if(status==GenericLexicalParser::END)
			return false;

		status=GenericLexicalParser::READY;
		
		while(status!=GenericLexicalParser::END){
			switch(q){
				case -1:
					//This is an error. The input is skipped untill an ' ','\n' or '\t' is found:
					switch(input->read()){
						case ' ':
						case '\n':
						case '\t':
						case 13:

							q=0;
							if(!input->next())status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_INVALID);
							return true;
						default:
							if(!input->next()){
								status=GenericLexicalParser::END;					
								return false;
							}
					}
					break;
				
				case 0:
					switch(input->read()){
						case ' ':
						case '\t':
						case '\n':
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;

						//Delimitadores
						case '(':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_ABRE_PAR);
							return true;
							break;
						case ')':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_CIERRA_PAR);
							return true;
							break;
						case '[':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_ABRE_COR);
							return true;
							break;
						case ']':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_CIERRA_COR);
							return true;
							break;
						case '{':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_ABRE_LLA);
							return true;
							break;
						case '}':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_CIERRA_LLA);
							return true;
							break;
						//Operadores:
						case '*':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_PRODUCTO);
							return true;
							break;
						case '/':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_DIVISION);
							return true;
							break;
						case '&':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_AND);
							return true;
							break;
						case '|':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_OR);
							return true;
							break;
						case '!':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_NOT);
							return true;
							break;
						case '+':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_SUMA);
							return true;
							break;
						case '-':
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_RESTA);
								return true;
							}
							numberSign=-1;
							q=13;
							break;


						//Separadores
						case ',':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_COMA);
							return true;
							break;
						case ';':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_PYC);
							return true;
							break;
						case '.':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_PUNTO);
							return true;
							break;
						case ':':
							if(!input->next())
								status=GenericLexicalParser::END;
							curToken=Token(GenericLexicalCathegories::T_DOS_PUNTOS);
							return true;
							break;

						//Datos
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
							//It can be a real or an integer.
                            value=(int)(input->read()-'0');
							q=17;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_ENTERO);
								v=new int;
								*v=value;
								curToken.values["value"]=v;
								return true;
							}
							break;
						case '\"':
							strIndex=0;
							q=24;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;
						default:
							if( (input->read()>='a' && input->read()<='z')
								||(input->read()>='A' && input->read()<='Z')
								||input->read()=='_'){
									//It is a reserved word!
									str[0]=input->read();
									strIndex=1;
									q=26;
									if(!input->next()){
										status=GenericLexicalParser::END;
										curToken=Token(GenericLexicalCathegories::T_RESERVED_WORD);
										str[strIndex]='\0';
										s= new char[strIndex+1];
										for(int i=0;i<=strIndex;i++)
											s[i]=str[i];
										curToken.values["string"]=s;			
										return true;
									}
								}
							else{
								q=-1;
								if(!input->next()){
									status=GenericLexicalParser::END;
									return false;
								}
							}

							break;


					}
					break;
				
				case 13:
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
							value=0;
							value=numberSign*(int)(input->read()-'0');//This is a negative number
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							q=14;
							break;
						default:
							curToken=Token(GenericLexicalCathegories::T_RESTA);
							return true;

					}
					break;

				case 14:
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
							
							value=10* value+numberSign*((int)(input->read()-'0'));
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_ENTERO);
								v=new int;
								*v=value;
								curToken.values["value"]=v;
								return true;								
							}
							break;
						case '.':
							//it can be a real
							//... just in case it is a real value:
							realValue=1.0f*value;
							weight=numberSign*0.1f;
							value=0;
							q=15;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;


						default:
							curToken=Token(GenericLexicalCathegories::T_ENTERO);
							v=new int;
							*v=value;
							curToken.values["value"]=v;
							return true;

					}
					break;
				case 15:
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
							//it IS  a real value:
							value=(int)(input->read()-'0');
							realValue=realValue+weight*value;
							weight=weight/10;
							//go to the next state:
							q=16;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_REAL);
								f=new float;
								*f=realValue;
								curToken.values["value"]=f;
								return true;
							}
							break;
						default://ERROR
							q=-1;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;

					}
					break;
				case 16:
					//It IS a real
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
							//It IS a real value
							value=(int)(input->read()-'0');
							realValue= realValue+weight*value;
							weight=weight/10;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_REAL);
								f=new float;
								*f=realValue;
								curToken.values["value"]=f;
								return true;
							}
							break;
						default:
							curToken=Token(GenericLexicalCathegories::T_REAL);
							f=new float;
							*f=realValue;
							curToken.values["value"]=f;
							return true;

					}
					break;
				case 17:
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
							//It IS an integer value
							value=10*value+(int)(input->read()-'0');
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_ENTERO);
								v=new int;
								*v=value;
								curToken.values["value"]=v;
								return true;
							}
							break;
						case '.':
							//It can be a real value or an OID:
							oid[0]=value;
							realValue=1.0f*value;
							weight=0.1f;
							q=18;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_ENTERO);
								v=new int;
								*v=value;
								curToken.values["value"]=v;
								return true;
							}
							value=0;
							break;
						default:
							curToken=Token(GenericLexicalCathegories::T_ENTERO);
							v=new int;
							*v=value;
							curToken.values["value"]=v;
							return true;

					}
					break;

				case 18:
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
							realValue= realValue+value*weight;
							weight=weight/10;
							q=19;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_REAL);
								f=new float;
								*f=realValue;
								curToken.values["value"]=f;
								return true;
							}
							break;
						default://ERROR
							q=-1;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;
					}

					break;
				case 19:
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
							realValue= realValue+weight*(int)(input->read()-'0');
							weight=weight/10;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_REAL);
								f=new float;
								*f=realValue;
								curToken.values["value"]=f;
								return true;
							}
							break;
						case '.':
							//It can be an OID:
							oid[1]=value;
							q=20;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_REAL);
								f=new float;
								*f=realValue;
								curToken.values["value"]=f;
								return true;
							}
							value=0;
							break;
						default:
							curToken=Token(GenericLexicalCathegories::T_REAL);
							f=new float;
							*f=realValue;
							curToken.values["value"]=f;
							return true;
					}
					break;
				case 20:
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
							q=21;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;								
							}
							break;
						default://ERROR
							q=-1;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;
					}
					break;
				case 21:
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
								status=GenericLexicalParser::END;
								
								return false;
							}
							break;
						case '.':
							//It can be an OID:
							oid[2]=value;
							q=22;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							value=0;
							break;
						default://ERROR
							q=-1;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;						
					}
					break;
				case 22:
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
							q=23;
							if(!input->next()){
								status=GenericLexicalParser::END;
								oid[3]=value;
								curToken=Token(GenericLexicalCathegories::T_OID);
								o=new OID();
								*o=oid;
								curToken.values["OID"]=o;
								return true;								
							}
							break;
						default://ERROR
							q=-1;
							if(!input->next()){
								status=GenericLexicalParser::END;
								return false;
							}
							break;
					}
					break;
				case 23:
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
								status=GenericLexicalParser::END;
								oid[3]=value;
								curToken=Token(GenericLexicalCathegories::T_OID);
								o=new OID();
								*o=oid;
								curToken.values["OID"]=o;
								return true;
							}
							break;
						default:
							oid[3]=value;
							curToken=Token(GenericLexicalCathegories::T_OID);
							o=new OID();
							*o=oid;
							curToken.values["OID"]=o;
							return true;
					}




					break;

				case 24:
					//parsing a string
					switch(input->read()){
						case '\"':
							q=0;
							curToken=Token(GenericLexicalCathegories::T_STRING);
							str[strIndex]='\0';
							//copy without "".
							s= new char[strIndex+1];
							for(int i=0;i<=strIndex;i++)
								s[i]=str[i];


							curToken.values["string"]=s;							
							if(!input->next()){
								status=GenericLexicalParser::END;								
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
								status=GenericLexicalParser::END;
								return false;
							}
							break;
					}
					break;
				case 26: 
					if( (input->read()>='a' && input->read()<='z')
						||(input->read()>='A' && input->read()<='Z')
						||(input->read()>='0' && input->read()<='9')
						||input->read()=='_'){
							//It is a reserved word!
							str[strIndex]=input->read();
							strIndex++;
							if(!input->next()){
								status=GenericLexicalParser::END;
								curToken=Token(GenericLexicalCathegories::T_RESERVED_WORD);
								str[strIndex]='\0';
								s= new char[strIndex+1];
								for(int i=0;i<=strIndex;i++)
									s[i]=str[i];
								curToken.values["string"]=s;			
								return true;
							}
						} else {
							curToken=Token(GenericLexicalCathegories::T_RESERVED_WORD);
							str[strIndex]='\0';
							s= new char[strIndex+1];
							for(int i=0;i<=strIndex;i++)
								s[i]=str[i];
							curToken.values["string"]=s;			
							return true;
						} 
					break;
			}
		}

	
		return false;
	}


};

#endif
