#ifndef _FIELD_DATA
#define _FIELD_DATA

#include "./FileReaders/BasicTypesReader.h"

class FieldData{
	std::string fieldName, value;
	std::list< FieldData*> subfields;
	std::list<FieldData*>::iterator itSubfields;
public:	
	FieldData(){
	}
	
	void setParameters(std::string fieldName,std::string value){
		this->fieldName=fieldName;
		this->value=value;
	}

	std::string getFieldName(){
		return fieldName;
	}

	std::string getFieldValue(){
		return value;
	}

	bool hasSubfield(std::string subfieldName){
		std::list<FieldData*>::iterator it;
		for(it=subfields.begin();it!=subfields.end();it++)
			if((*it)->getFieldName()==subfieldName)
				return true;
		return false;
	}

	bool addSubfield(FieldData* f){
		if(f==NULL)
			return false;//PREVENT NULL-POINTER EXCEPTIONS
		//if(this->hasSubfield(f->getFieldName()))
		//	return false; //THIS FIELD ALREADY EXISTS!!
		//Add the subfield
		this->subfields.push_back(f);
		return true;
	}

	FieldData* getSubfield(std::string subfieldName){
		std::list<FieldData*>::iterator it;
		for(it=subfields.begin();it!=subfields.end();it++)
			if((*it)->getFieldName()==subfieldName)
				return *it;
		return NULL;
	}

	void resetSubfields(){
		this->itSubfields=subfields.begin();
	}

	bool hasNextSubfield(){
		return this->itSubfields!=subfields.end();
	}

	FieldData* getNextSubfield(){
		FieldData* result=*itSubfields;
		itSubfields++;
		return result;
	}

};
#endif