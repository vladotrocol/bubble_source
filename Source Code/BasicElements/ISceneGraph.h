#ifndef _ISCENE_GRAPH
#define _ISCENE_GRAPH

#include "./IObject.h"
#include "./Ogre.h"

using namespace Ogre;

class ISceneGraph{
public:

	//SCENEGRAPH MANAGEMENT
	virtual bool definePrototype(IObject* proto, std::string protoId)=0;

	virtual bool removePrototype(std::string protoId)=0;

	//OBJECT MANAGEMENT
	virtual IObject* addObject(OID oid, IObject* o)=0;

	virtual IObject* addObject(IObject* o)=0;
	
	virtual IObject* addObjectByPrototype(OID oid,std::string protoId)=0;
	
	virtual IObject* addObjectByPrototype(std::string protoId)=0;

	virtual bool removeObject(OID oid)=0;
	
	virtual bool isAntecesorAofB(OID a,OID b)=0;

	virtual bool attachTo(OID target,OID pOID)=0;

	virtual void setPos(OID oid, Matrix4 pos)=0;

	virtual void setStatus(OID oid, int status)=0;

	virtual Ogre::Matrix4 getTransformationMatrixFromAToB(OID a,OID b)=0;

	//Gesti�n del flujo de ejecuci�n
	virtual std::list<Interaction*>* checkActions(std::list<Action*>* al)=0; 

	virtual void commitOperations(std::list<Operation*>* ol)=0;
		
	virtual void nextFrame(float time)=0;

	//Obtener estructura del grafo para OutputManager
	virtual std::list<Operation*>* getSceneStructure()=0;

	virtual std::list<Operation*>* getSceneRepresentations(int channel)=0;

	virtual int numChannels()=0;


};

#endif


