#ifndef _LEXICAL_PARSER
#define _LEXICAL_PARSER

#include "./FileReaders/InputStream.h"
#include "./FileReaders/Token.h"

class LexicalParser{
protected:
	InputStream* input;

public:

	virtual Token getToken()=0;

	virtual bool nextToken()=0;

};


#endif
