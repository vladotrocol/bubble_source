#ifndef _PLUG_IN
#define _PLUG_IN
#include <string>
class IEventProducer;

class IPlugin{
public:
	virtual void config(IEventProducer& mainApp)=0;
	virtual std::string getHelpString()=0;
};


#endif