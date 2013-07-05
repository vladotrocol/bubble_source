#include "./FileReaders/GenericLexicalParser.h"
#include "./FileReaders/InteractionReaders/InteractionParser.h"
#include "./FileReaders/OperationListReaders/OperationListReader.h"
//#include "./CollaborationManager/RaknetCollaborationManager.h"

/*
//COMPROBACI�N NIVEL SEM�NTICO: fICHERO testSemantical.txt
int main(){
	NodeDefinitionSemanticalParser p("testSemantical.txt");
	if(p.parse())
		printf("BIEN!!");
	else
		printf("MAL!!");
	return 1;

}
*/

//COMPROBACI�N NIVEL L�XICO: fICHERO testLexical.txt

int main(){
	GenericLexicalParser p("testLexical.txt");
	Token t(-1);
	while(p.nextToken()){
		t=p.getToken();
		printf("\nToken(%d",t.tokenId);
		std::map<std::string,void*>::iterator it;
		for(it=t.values.begin();it!=t.values.end();it++){
			std::string id=it->first;
			OID oid=*((OID*)it->second);
			int value=*((int*)it->second);
			char* str=((char*)it->second);
			printf(",%s",it->first.c_str());
			
		}
		printf(");");
	}
	return 1;

}

