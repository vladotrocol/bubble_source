#ifndef _IOBJECT
#define _IOBJECT

#include "./BasicTypes.h"
#include "./Action.h"
#include "./Interaction.h"
#include "./SceneGraph/InteractionView.h"
#include "./Operation.h"

class IObject{
protected:
	bool localObject;
public:
	OID oid;
	OID pOID;
	Ogre::Matrix4 pos;
	std::list<IObject*> children;
	int status;

	/**
		initializeReources-> Este m�todo se invoca cuando se crea una copia objetiva del objeto para inicialiar los recursos 
		necesarios (ficheros, dispositivos reales,...) 
	*/
	virtual void initializeResources()=0;


	bool isLocalObject(){
		return localObject;
	}

	/*
		Este m�todo demanda la exclusi�n m�tua sobre el objeto.
	*/
	virtual void lock()=0;

	/*
		Este m�todo libera la exclusi�n m�tua sobre el objeto.
	*/

	virtual void unlock()=0;

	virtual void attachTo(OID oid)=0;

	virtual void dettach()=0;

	virtual void setPos(Ogre::Matrix4 newPos)=0;

	virtual void setStatus(int st)=0;

	virtual void moveRepresentation(ID targetIV,ID representationName,Ogre::Matrix4 pos)=0;

	virtual void addRepresentation(std::string targetIV,InteractionView* iv)=0;

	virtual std::list<Interaction*>* checkActions(std::list<Action*>* actions,Ogre::Matrix4 fromRootToThis=Ogre::Matrix4::IDENTITY)=0;

	virtual std::list<Operation*>* commitInteraction(Interaction* i)=0;

	virtual void nextFrame(float time)=0;

	virtual void filterActions(std::list<Action*>* actions,std::list<AID>* actionsModified){;}

	virtual IObject* copy()=0;

	virtual std::list<Operation*>* getRepresentations(int channel)=0;

	virtual void addChild(OID childOID)=0;

	virtual void removeChild(OID oid)=0;

	virtual std::list<Operation*>* getSceneStructure()=0;

	virtual std::list<Operation*>* getSceneRepresentations(int channel)=0;

	virtual IObject* getChild(int num)=0;

	virtual InteractionView* getInteractionView(ID id)=0;
};

#endif


