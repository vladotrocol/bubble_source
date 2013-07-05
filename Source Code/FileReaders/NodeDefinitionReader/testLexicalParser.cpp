#include "./FileReaders/NodeDefinitionReader/NodeDefinitionSemanticalParser.h"
#include "./CollaborationManager/RaknetCollaborationManager.h"

/*
//COMPROBACIÓN NIVEL SEMÄNTICO: fICHERO testSemantical.txt
int main(){
	NodeDefinitionSemanticalParser p("testSemantical.txt");
	if(p.parse())
		printf("BIEN!!");
	else
		printf("MAL!!");
	return 1;

}
*/
/*
//COMPROBACIÓN NIVEL LÉXICO: fICHERO testLexical.txt

int main(){
	NodeDefinitionLexicalParser p("testLexical.txt");
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
*/