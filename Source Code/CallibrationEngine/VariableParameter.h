#ifndef _VARIABLE_PARAMETER
#define _VARIABLE_PARAMETER

class IVariableParameter{
public:
	virtual void increase()=0;
	virtual void decrease()=0;
	virtual void set(float normalizedValue)=0;
	virtual ~IVariableParameter(){;}
};

class IntegerParameter:public IVariableParameter{
int min, max, step, curValue;
public:
	IntegerParameter(int min=0, int max=255, int step=1): curValue(min),min(min), max(max),step(step){;}
	
	virtual void increase(){
		if(curValue+step<max) curValue+=step;
		else curValue=max;
	}

	virtual void decrease(){
		if(curValue-step>min)
			curValue-=step;
		else curValue=min;
	}

	virtual void set(float normalizedValue){
		if(normalizedValue<0 || normalizedValue>=1)
			return; //Value has to be 0=< value<1
		curValue=(int)(min + (max-min)*normalizedValue);
	}

	inline int get(){
		return curValue;
	}
};

class FloatParameter: public IVariableParameter{
float min, max, step, curValue;
public:
	FloatParameter(float min=0, float max=255, float step=1): curValue(min),min(min), max(max),step(step){;}
	
	virtual void increase(){
		if(curValue+step<max) curValue+=step;
		else curValue=max;
	}

	virtual void decrease(){
		if(curValue-step<min)curValue-+step;
		else curValue=min;
	}

	virtual void set(float normalizedValue){
		if(normalizedValue<0 || normalizedValue>=1)
			return; //Value has to be 0=< value<1
		curValue=(float)(min + (max-min)*normalizedValue);
	}

	inline float get(){
		return curValue;
	}
};

#endif

