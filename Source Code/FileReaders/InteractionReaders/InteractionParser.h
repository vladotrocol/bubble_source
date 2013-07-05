#ifndef _INTERACTION_PARSER
#define _INTERACTION_PARSER

#include "./FileReaders/SemanticalParser.h"
#include "./FileReaders/GenericLexicalParser.h"
#include "./CommunicationElements/CommunicationLayer.h"




using namespace GenericLexicalCathegories;

class InteractionParser: public SemanticalParser{
Token curToken;
Interaction* data;

public:
	InteractionParser(InputStream* input):curToken(-1){
		this->input=new GenericLexicalParser(input);
	}

	~InteractionParser(){
		if(input)delete input;
	}
	//Parses an interaction from the InputStream and returns true if the deffinition is correct.
	virtual bool parse(){		
		return S();
	}
	
	//Gets the interaction parsed from the inputStream.
	Interaction* getData(){
		return data;
	}

	//Writes the Interaction into a string.
	static char* putData(Interaction* i){
		std::string result="";

		char buffer[10024];
		
		//Interation ( priority <Number>; producerOID OID; producerIVID STRING;...
		sprintf(buffer,"Interaction ( \n\tpriority %u;\n\t producerOID %s;\n\t producerIVID \"%s\";\n\t",i->priority,i->producerOID.toDotString(),i->producerIVID.c_str());
		result.append(buffer);

		//<ActionList>-> ActionList[<AL>];
		sprintf(buffer,"\n\tActionList[");
		result.append(buffer);
		std::list<Action*>::iterator it;
		for(it=i->trigger.begin();it!=i->trigger.end();it++){
			Action* curAction=*it;
			sprintf(buffer,"\n\t\tAction (aid(%s/%u);weight %5f;\n\t\t\tContextList[", curAction->aid.oid.toDotString(), curAction->aid.code,curAction->weight);
			result.append(buffer);
			//<ContextList>
			std::list<Context*>::iterator itContexts;
			for(itContexts=curAction->context.begin();itContexts!=curAction->context.end();itContexts++){
				Context* curContext=*itContexts;
				char matrix_str[512];
				sprintf(matrix_str,"[%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;%10f;]", 1.0*curContext->pos[0][0], 1.0*curContext->pos[0][1], 1.0*curContext->pos[0][2], 1.0*curContext->pos[0][3], 1.0*curContext->pos[1][0], 1.0*curContext->pos[1][1], 1.0*curContext->pos[1][2], 1.0*curContext->pos[1][3], 1.0*curContext->pos[2][0], 1.0*curContext->pos[2][1], 1.0*curContext->pos[2][2], 1.0*curContext->pos[2][3], 1.0*curContext->pos[3][0], 1.0*curContext->pos[3][1], 1.0*curContext->pos[3][2], 1.0*curContext->pos[3][3]);
				sprintf(buffer,"Context(id \"%s\"; inputChannel %u; startTime %f; endTime %f; dumpTime %f; possition %s; radius %f; rootOID %s; mask %s;);\n\t\t\t\t",curContext->id.c_str(), curContext->inputChannel, curContext->startTime, curContext->endTime, curContext->dumpTime, matrix_str, curContext->radius, curContext->rootOID.toDotString(), curContext->mask.toDotString());
				result.append(buffer);

			}
			sprintf(buffer,"\n\t\t\t];");//fin ContextList
			result.append(buffer);

			//<AdditionalData>
			sprintf(buffer,"\n\t\t\tAdditionalData [");
			result.append(buffer);

			std::map<std::string,std::string>::iterator itAddData;
			for(itAddData=curAction->additionalData.begin();itAddData!=curAction->additionalData.end();itAddData++){
				sprintf(buffer,"[ \"%s\" / \"%s\"];",itAddData->first.c_str(),itAddData->second.c_str());
				result.append(buffer);			
			}
			
			sprintf(buffer,"\n\t\t\t];");// fina dditional Data
			result.append(buffer);			

			sprintf(buffer,"\n\t\t);");//Fin de la acci�n
			result.append(buffer);			

		}
		sprintf(buffer,"\n\t];");//Fin ActionList
		result.append(buffer);
		//<AdditionalData>
		sprintf(buffer,"\n\tAdditionalData [");
		result.append(buffer);

		std::map<std::string,std::string>::iterator itAddData;
		for(itAddData=i->additionalData.begin();itAddData!=i->additionalData.end();itAddData++){
			sprintf(buffer,"[ \"%s\" / \"%s\"];",itAddData->first.c_str(),itAddData->second.c_str());
			result.append(buffer);			
		}
		
		sprintf(buffer,"\n\t];\n);\n\n");// fin additional Data+fin Interacci�n
		result.append(buffer);	

		char*resultChar=new char[result.size()+1];
		strcpy(resultChar,result.c_str());
		return resultChar;
	}
private:
	
	//<S>-->Interaction ( priority ENTERO; producerOID OID; producerIVID STRING; <ActionList> <AdditionalData>);eof
	bool S(){
		data=new Interaction();
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="Interaction")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_PAR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="priority")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_ENTERO)return false;
		this->data->priority=*(int*)(curToken.values["value"]);
		delete (int*)curToken.values["value"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="producerOID")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_OID)return false;
		this->data->producerOID=*(OID*)curToken.values["OID"];
		delete (OID*)curToken.values["OID"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="producerIVID")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_STRING)return false;
		this->data->producerIVID=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		if(!ActionList())
			return false;

		if(!AdditionalData(&this->data->additionalData))
			return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_PAR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		return true;
	}

	//<ActionList>-> ActionList [ <AL> ] ;

	bool ActionList(){
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="ActionList")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		if(!AL())return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		return true;
	}

	//<AL>--> lambda| <ParseAction> <AL>
	
	bool AL(){
		
		curToken=input->getToken();
		
		if(curToken.tokenId==T_CIERRA_COR)return true; //<AL>-> lambda

		if(curToken.tokenId==T_RESERVED_WORD) //<AL>-> <Action><AL>
			return ParseAction() && AL();
		return false; 	
	}
	//<Action>-> Action ( aid(OID/OID);weight <Number>; <ContextList> <AdditionalData>);
	bool ParseAction(){
		Action* a =new Action();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="Action")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_PAR)return false;
		input->nextToken();

		//aid (OID/ENTERO)
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="aid")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_PAR)return false;
		input->nextToken();

		OID rootOID;
		int code;
		curToken=input->getToken();
		if(curToken.tokenId!=T_OID)return false;
		rootOID=*(OID*)curToken.values["OID"];
		delete (OID*)curToken.values["OID"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_DIVISION)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ENTERO)return false;
		code=*(int*)curToken.values["value"];
		delete (int*)curToken.values["value"];
		input->nextToken();

		
		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_PAR)return false;
		input->nextToken();

		
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		a->aid=AID(rootOID,code);

		//weight <Number>;
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="weight")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		float weight;
		if(!Number(&weight))
			return false;
		a->weight=weight;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		if(!ContextList(&a->context)) 
			return false;

		if (!AdditionalData(&a->additionalData))
			return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_PAR)return false;
		input->nextToken();
				
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		//A�adir la acci�n leida:
		this->data->trigger.push_back(a);


		return true;
	}

	bool Number(float* f){
		curToken=input->getToken();
		if(curToken.tokenId==T_ENTERO){
			*f=(*(int*)curToken.values["value"])*1.0;
			delete 	(int*)curToken.values["value"];
			input->nextToken();
			return true;
		}
		else if(curToken.tokenId==T_REAL){
			*f=(*(float*)curToken.values["value"])*1.0;
			delete 	(float*)curToken.values["value"];
			input->nextToken();
			return true;
		}else return false;	
	}

	//<ContextList> --> ContextList [<CL>];
	bool ContextList(std::list<Context*>* a){
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="ContextList")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		if(!CL(a))
			return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();


		return true;
	}

	/*<CL>--> lambda 
			| Context ( id STRING; inputChannel entero; 
			startTime <Number>; endTime <Number>; dumpTime <Number>;
			possition <Matrix> ; radius <Number>;
			rootOID OID; mask OID; ); <CL>
	*/

	bool CL (std::list<Context*>* a){
		
		//<CL>--> lambda
		curToken=input->getToken();
		if(curToken.tokenId==T_CIERRA_COR)return true; 
		
		//Context ( id STRING; inputChannel entero; .... -vayamos por partes-
		Context* ctx=new Context();
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="Context")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_PAR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="id")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		curToken=input->getToken();
		if(curToken.tokenId!=T_STRING)return false;
		ctx->id=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();

				
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="inputChannel")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ENTERO)return false;
		ctx->inputChannel=*(int*)curToken.values["value"];
		delete (int*)curToken.values["value"];
		input->nextToken();

		
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		//...starTime <Number>; endTime <Number>; dumpTime <Number>;...
		float startTime, endTime, dumpTime;

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="startTime")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		if(!Number(&startTime))return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="endTime")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		if(!Number(&endTime))return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="dumpTime")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		if(!Number(&dumpTime))return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		ctx->startTime=startTime;
		ctx->endTime=endTime;
		ctx->dumpTime=dumpTime;

		//... possition <Matrix>; radius <Number>;...
		Ogre::Matrix4 pos;
		float radius;

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="possition")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		if(!Matrix(&pos))
			return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="radius")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();


		if(!Number(&radius))
			return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		ctx->pos=pos;
		ctx->radius=radius;
		
		//...rootOID OID; mask OID; );
		OID rootOID, mask;
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="rootOID")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_OID)return false;
		rootOID=*(OID*)curToken.values["OID"];
		delete (OID*)curToken.values["OID"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="mask")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();
		
		curToken=input->getToken();
		if(curToken.tokenId!=T_OID)return false;
		mask=*(OID*)curToken.values["OID"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		ctx->rootOID=rootOID;
		ctx->mask=mask;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_PAR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();
		a->push_back(ctx);
		
		return CL(a);
	}



	//<AdditionalData>-> AdditionalData (<DataList>);
	bool AdditionalData(std::map<std::string,std::string> *a){
		Context* ctx=new Context();
		curToken=input->getToken();
		if(curToken.tokenId!=T_RESERVED_WORD||std::string((char*)curToken.values["string"])!="AdditionalData")return false;
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		if(!DataList(a))return false;

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		return true;
	}

	//<DataList>-> lambda | [string / string]; <DataList>
	bool DataList(std::map<std::string,std::string> *a){
		//<DataList>--> lambda
		curToken=input->getToken();
		if(curToken.tokenId==T_CIERRA_COR)return true;

		//<DataList>-->[string / string]; <DataList>
		std::string first,second;
		curToken=input->getToken();
		if(curToken.tokenId!=T_ABRE_COR)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_STRING)return false;
		first=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();


		curToken=input->getToken();
		if(curToken.tokenId!=T_DIVISION)return false;
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_STRING)return false;
		second=std::string((char*)curToken.values["string"]);
		delete (char*)curToken.values["string"];
		input->nextToken();

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();

		
		curToken=input->getToken();
		if(curToken.tokenId!=T_PYC)return false;
		input->nextToken();

		//Attribute-value pair parsed--> add it!
		(*a)[first]=second;

		return DataList(a);



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
				if(curToken.tokenId!=T_PYC)return false;
				input->nextToken();

				(*pos)[i][j]=value_i_j;
			}

		curToken=input->getToken();
		if(curToken.tokenId!=T_CIERRA_COR)return false;
		input->nextToken();
				
		return true;
	
	}
};

#endif

