#pragma once

#include "rtti.h"
#include "event.h"
#include "node.h"
#include "rCollisionMonitor.h"


#define Declare_Entity_Creator(classname) static Kylin::Entity * CreateInstance() {return KNEW classname;}


///This is the root of all the entity class hierarchy
namespace Kylin
{
	class Entity : public Node
				 , public EventHandler
	{
	public:
		BtDeclareRootRTTI(Entity)
		Declare_Event_Handler(Entity, EventHandler)
		Declare_Entity_Creator(Entity)
		Entity(void);
		virtual ~Entity(void);

	public:
		virtual KBOOL	Init(const PropertySet& kProp);
		virtual KVOID	Tick(KFLOAT fElapsed);
		virtual KVOID	SetupCllsn(PropertySet kProp);


		KVOID	SetID(KUINT uID) { m_uID = uID; }
		KUINT	GetID() const { return m_uID; }
		
		KVOID	SetName(KSTR sName) { m_sName = sName; }
		KCCHAR*	GetName() const { return m_sName.data(); }

		PropertySet&	GetPropertyRef() { return m_kProperty; }
	
	public:
		virtual KVOID OnEntityCllsn(Entity* pCollidee,const KPoint3& rNormal){}
		virtual KBOOL OnShouldCllsn(Entity* pCollidee){ return false; }

	protected:
		KVOID	EV_PostDestroy(EventPtr spEV);
		KVOID	EV_PostSpawn(EventPtr spEV);

	protected:
		virtual KVOID	PostSpawn();
		virtual KVOID	PostDestroy();

	protected:
		KUINT		m_uID;
		KSTR		m_sName;
		PropertySet m_kProperty;
	};
	
}

