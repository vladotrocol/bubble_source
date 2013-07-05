/*******************************************************************************************************
*			BASIC TYPES
********************************************************************************************************
*  	Diego Martinez Plasencia, LoUISE	
********************************************************************************************************
*	REVISIONES:
*	  18/09/07--> Versión inicial			
********************************************************************************************************
*	This file contains a simple main to test the AID class
*******************************************************************************************************/



#include "./BasicTypes.h"



#include <stdio.h>
int main(void){
	AID a,b;
	char opc;
	do{
		fflush(stdin);		
		printf("--AIDS TEST PROGRAM:\n");
		printf("\t1.Crear AID a.\n");
		printf("\t2.Crear AID b.\n");
		printf("\t3.Copiar a en b.\n");
		printf("\t4.Incrementar a.\n");		
		printf("\t5.Incrementar b.\n");		
		printf("\t6.Iguales a y b?\n");		
		printf("\t7.Distintos a y b?\n");
		printf("\ts. Salir\n");
		scanf("%c",&opc);
		switch(opc){
			case '1':	
				break;
			case '2':
				break;
			case '3':
				b=a;	
				printf("a=[ (%c,%c,%c,%c), %d ]\n",a.oid[0],a.oid[1],a.oid[2],a.oid[3], a.code);
				printf("b=[ (%c,%c,%c,%c), %d ]\n",b.oid[0],b.oid[1],b.oid[2],b.oid[3], b.code);
				break;		
			case '4':
				++a;
				printf("a=[ (%c,%c,%c,%c), %d ]\n",a.oid[0],a.oid[1],a.oid[2],a.oid[3], a.code);
				break;
			case '5':
				++b;
				printf("b=[ (%c,%c,%c,%c), %d ]\n",b.oid[0],b.oid[1],b.oid[2],b.oid[3], b.code);
				break;
			case '6':
				if(a==b)printf("\na es igual que b\n");
				else printf("\na es distinto que b\n");				
				break;
			case '7':
				if(a!=b)printf("\na es distinto que b\n");
				else printf("\na es igual que b\n");
				break;
				
			default:
				break;
		}

								
		

	}while(opc!='s');


}



