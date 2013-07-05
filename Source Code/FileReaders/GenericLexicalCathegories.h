#ifndef _GENERIC_LEXICAL_CATHEGORIES
#define _GENERIC_LEXICAL_CATHEGORIES

namespace GenericLexicalCathegories{

	//Delimitadores
	static const int T_INVALID =0;
	static const int T_ABRE_PAR=1;
	static const int T_CIERRA_PAR=2;
	static const int T_ABRE_COR=3;
	static const int T_CIERRA_COR=4;
	static const int T_ABRE_LLA=5;
	static const int T_CIERRA_LLA=6;

	//Operadores
	static const int T_SUMA=7;
	static const int T_RESTA=8;
	static const int T_PRODUCTO=9;
	static const int T_DIVISION=10;
	static const int T_AND=11;
	static const int T_OR=12;
	static const int T_NOT=13;
	static const int T_MODULO=14;
	static const int T_DOLAR=15;

	//Separador
	static const int T_PYC=16;
	static const int T_COMA=17;
	static const int T_PUNTO=18;
	static const int T_DOS_PUNTOS=19;

	//Datos
	static const int T_ENTERO=20;
	static const int T_REAL=21;
	static const int T_OID=22;
	static const int T_STRING=23;
	static const int T_RESERVED_WORD=24;

}

#endif
