/*******************************************************************************************************
*			BASIC TYPES
********************************************************************************************************
*  	Diego Martinez Plasencia, LoUISE	
********************************************************************************************************
*	REVISIONES:
*	  19/09/07--> Versión inicial			
********************************************************************************************************
*	This file contains a simple main to test the Period class
*******************************************************************************************************/



#include "./BasicTypes.h"



#include <stdio.h>
int main(void){
	Period a,b;
	float sT,eT,dT;
	int opc;
	do{
		fflush(stdin);
		printf("\n\n--PERIOD TEST PROGRAM:\n");
		printf("\t1.Crear Period a.\n");
		printf("\t2.Crear Period b.\n");
		printf("\t3.Comprobar a.\n");
		printf("\t4.Comprobar b.\n");		
		printf("\t5.¿Es subconjunto a de b?.\n");		
		printf("\t6.¿Se solapan a y b?\n");		
		printf("\t7.¿Son disjuntos a y b?\n");
		printf("\t8.¿Son adyacentes a y b?\n");
		printf("\t9.¿Es a anterior a b?\n");
		printf("\t10.¿Es a posterior a b?\n");
		printf("\t11. Salir\n");
		scanf("%d",&opc);
		switch(opc){
			case 1:	
				printf("\nIntroduzca comienzo:");
				scanf("%f",&sT);
				printf("\nIntroduzca fin:");
				scanf("%f",&eT);
				printf("\nIntroduzca margen:");
				scanf("%f",&dT);
				a=Period(sT,eT,dT,"a");	
				break;
			case 2:
				printf("\nIntroduzca comienzo:");
				scanf("%f",&sT);
				printf("\nIntroduzca fin:");
				scanf("%f",&eT);
				printf("\nIntroduzca margen:");
				scanf("%f",&dT);
				b=Period(sT,eT,dT,"b");
				break;
			case 3:
				if(Period::isCorrect(a))printf("\nPeriodo a(%5f,%5f,%5f,%s) es correcto\n",a.startTime,a.endTime,a.dumpTime,a.id);
				else printf("\nPeriodo a(%5f,%5f,%5f,%s) es incorrecto\n",a.startTime,a.endTime,a.dumpTime,a.id);
				break;		
			case 4:
				if(Period::isCorrect(b))printf("\nPeriodo b(%5f,%5f,%5f,%s) es correcto\n",b.startTime,b.endTime,b.dumpTime,b.id);
				else printf("\nPeriodo b(%5f,%5f,%5f,%s) es incorrecto\n",b.startTime,b.endTime,b.dumpTime,b.id);
				break;				
			case 5:
				if(Period::subset(a,b))printf("\n a es subconjunto de b\n");
				else printf("\n a no es subconjunto de b\n");
				break;
			case 6:
				if(Period::overlap(a,b))printf("\n a se solapa con b\n");
				else printf("\n a no se solapa con b\n");
				break;
			case 7:
				if(Period::disjoint(a,b))printf("\n a es disjunto con b\n");
				else printf("\n a no es disjunto con b\n");
				break;
			case 8:
				if(Period::adyacent(a,b))printf("\n a es adyacente a b\n");
				else printf("\n a no es adyacente a b\n");
				break;				
			case 9:
				if(Period::previous(a,b))printf("\n a es anterior a b\n");
				else printf("\n a no es anterior a b\n");
				break;				
			case 10:
				if(Period::later(a,b))printf("\n a es posterior a b\n");
				else printf("\n a no es posterior a b\n");
				break;				
			default:
				break;
		}

								
		

	}while(opc!=11);


}
