#ifndef _GENERIC_TYPE
#define _GENERIC_TYPE
/*
static int numTypes=0;

class GenericType{
public:	
	void* data;
	GenericType(void* data){
		this->data=data;
		numTypes++;
		printf("\nGenericTypes:%d",numTypes);
	}
	~GenericType(){
		delete data;
	}
	virtual GenericType* copy()=0;
};

class GenericString:public GenericType{
public:
	//data must be a string*
	GenericString(void* data): GenericType(data){
		;
	}

	~GenericString(){
		delete ((std::string*)data);
		numTypes--;
	}

	GenericType* copy(){
		GenericString* result=new GenericString((void*)new std::string);
		*((std::string*)result->data)=*((std::string*)this->data);
		return result;
	}
};

class GenericInteger:public GenericType{
public:
	GenericInteger(void* data): GenericType(data){;}
	~GenericInteger(){
		delete ((int*)data);
		numTypes--;
	}
	GenericType* copy(){
		GenericInteger* result=new GenericInteger((void*)new int);
		*((int*)result->data)=*((int*)this->data);
		return result;
	}
};
/*template<class T>
class TestGeneric{
protected:
	T * data;
public:
	TestGeneric(T * data){
		this->data=data;
	}

	~TestGeneric(){
		delete data;
	}

	TestGeneric<T>* copy(){
		TestGeneric<T>* result= new TestGeneric<T>(new T());
		*(result->data)=*this->data;
		return result;
	}
};*/

#endif
