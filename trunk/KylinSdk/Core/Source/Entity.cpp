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
: m_uID()
{
	 
}

Kylin::Entity::~Entity( void )
{

}

KBOOL Kylin::Entity::Init( const PropertySet& kProp )
{
	assert(kProp.GetCount() > 0);
	m_kProperty = kProp;
	
	if (!Node::Load(kProp))
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

