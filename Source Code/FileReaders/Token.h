#ifndef _TOKEN
#define _TOKEN

#include <iostream>
#include <string>
#include <map>

class Token{
public:
	int tokenId;
	std::map<std::string,void*> values;
	Token(int tokenId){
		this->tokenId=tokenId;
	}
};
#endif
