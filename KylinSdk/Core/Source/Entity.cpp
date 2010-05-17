#include "corepch.h"
#include "Entity.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"


namespace Kylin
{
	BtImplementRootRTTI(Entity, id_entity);

	Implement_Event_Handler(Entity, EventHandler)
	{
		{&ev_post_destroy,			&EV_PostDestroy},
		{&ev_post_spawn,			&EV_PostSpawn},
		{NULL, NULL}
	};
}

Kylin::Entity::Entity( void )
: m_uID(0xffffffff)
{
	 
}

Kylin::Entity::~Entity( void )
{

}

KBOOL Kylin::Entity::Init( const PropertySet& kProp )
{
	assert(kProp.GetCount() > 0);
	m_kProperty = kProp;
	m_kProperty.SetValue("$ID",m_uID);

	if (!Node::Load(m_kProperty))
		return false;
	
	//////////////////////////////////////////////////////////////////////////
	EventPtr spEV(
		new Event(
		&ev_post_spawn, 
		Event::ev_nextframe, 
		0, 
		0, 
		NULL
		));

	KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);

	return true;
}

KVOID Kylin::Entity::PostSpawn()
{
	
}

KVOID Kylin::Entity::Tick( KFLOAT fElapsed )
{
	Node::Tick(fElapsed);

}

KVOID Kylin::Entity::EV_PostDestroy( EventPtr spEV )
{
	PostDestroy();
}

KVOID Kylin::Entity::PostDestroy()
{
	KylinRoot::GetSingletonPtr()->DestroyEntity(this->GetID());
}

KVOID Kylin::Entity::EV_PostSpawn( EventPtr spEV )
{
	PostSpawn();
}