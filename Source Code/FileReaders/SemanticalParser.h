#ifndef _SEMANTICAL_PARSER
#define _SEMANTICAL_PARSER

#include "./FileReaders/LexicalParser.h"
#include "./FileReaders/Token.h"

class SemanticalParser{
protected:
	LexicalParser* input;

public:

	virtual bool parse()=0;

};
#endif
