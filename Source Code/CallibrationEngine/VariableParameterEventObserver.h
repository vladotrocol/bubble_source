#ifndef _VARIABLE_PARAMETER_EVENT_OBSERVER
#define _VARIABLE_PARAMETER_EVENT_OBSERVER
#include "./CallibrationEngine/IEventObserver.h"
#include "./CallibrationEngine/VariableParameter.h"

class OnEventIncreaseParameter:public IEventObserver{
	IVariableParameter& proxy;
public:
	OnEventIncreaseParameter(IVariableParameter& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.increase();
	}
};

class OnEventDecreaseParameter:public IEventObserver{
	IVariableParameter& proxy;
public:
	OnEventDecreaseParameter(IVariableParameter& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.decrease();
	}
};

#endif