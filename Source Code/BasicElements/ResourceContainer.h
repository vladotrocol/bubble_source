#ifndef _RESOURCE_CONTAINER
#define _RESOURCE_CONTAINER
#include <string>

class ResourceContainer{

public:
	virtual int numResources()=0;
	virtual std::string getResourceName(int resourceNumber)=0;
	virtual void* getResource(std::string resourceName)=0;

};

#endif