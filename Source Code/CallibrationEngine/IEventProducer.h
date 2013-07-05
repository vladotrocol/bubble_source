#ifndef _I_EVENT_PRODUCER
#define _I_EVENT_PRODUCER
#include <map>
#include <list>
#include <vector>
#include <string>
#include "./CallibrationEngine/IEventObserver.h"
#include "./CallibrationEngine/IPlugin.h"

class IEventProducer{
	std::map<char, std::list<IEventObserver*>> observers;
	std::vector<IPlugin*> plugins;
public:
	virtual void registerEventObserver(char key,IEventObserver* o){
		observers[key].push_back(o);
	}
	virtual void unregisterEventObserver(char key, IEventObserver* o){
		std::list<IEventObserver*>::iterator it;
		for(it=observers[key].begin();it!=observers[key].end();it++)
			if(*it==o){
				observers[key].erase(it);
				return;
			}
	}
	inline void notifyObservers(char key){
		std::list<IEventObserver*>::iterator it;
		for(it=observers[key].begin();it!=observers[key].end();it++)
			(*it)->notifyEvent();
	}
	inline void registerPlugin(IPlugin* i){
		plugins.push_back(i);
		i->config(*this);
	}
	std::vector<std::string> getHelpStrings(){
		std::vector<std::string> result;
		for(unsigned int i=0;i<plugins.size();i++)
			result.push_back(plugins[i]->getHelpString());
		return result;
	}
};

#endif