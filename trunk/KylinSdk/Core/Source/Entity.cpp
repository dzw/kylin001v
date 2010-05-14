#include "corepch.h"
#include "Entity.h"
#include "RegisterClass.h"


namespace Kylin
{
	BtImplementRootRTTI(Entity, id_entity);

	Implement_Event_Handler(Entity, EventHandler)
	{
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
	
	return true;
}

KVOID Kylin::Entity::PostSpawn()
{
	
}

KVOID Kylin::Entity::Tick( KFLOAT fElapsed )
{
	Node::Tick(fElapsed);

}

