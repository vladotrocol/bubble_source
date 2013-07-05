#include "./FileReaders/GenericConfigReader/GenericConfigReader.h"
#include "./InputSubsystem/VRJugglerInputSubsystem/VRJugglerInputSubsystem.h"
#include <stdio.h>

int main(){
	VRJugglerInputSubsystem s;
	s.configure("VRJugglerConfig.cfg");
	s.startInputSubsystem();
	char c;
	scanf("%c",&c);
	s.stopInputSubsystem();
}

/*void show(FieldData c,int count);
int main(){
	printf("hello world");
	GenericConfigReader c(new FileInputStream("VRJugglerConfig.cfg"));
	if(c.parse())
		printf("Ole!!!!!");
	else 
		printf("·$&%$/%·!!");
	char ch;
	show(c.configuration,0);
	scanf("%c",&ch);
}

void show(FieldData c,int count){
	printf("\n");
	for(int i=0;i<count;i++)
		printf("   ");
	printf("%s %s",c.getFieldName().c_str(),c.getFieldValue().c_str());
	c.resetSubfields();
	while(c.hasNextSubfield())
		show(*(c.getNextSubfield()),count+1);
	
}*/