#include "corepch.h"
#include "Character.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "ScriptVM.h"

#include "ActionDispatcher.h"
#include "rOgreUtils.h"
#include "Avatar.h"

#define CH_HEIGHT 1.0f;

namespace Kylin
{
	BtImplementRTTI(Character, Entity, id_character);

	Implement_Event_Handler(Character, Entity)
	{
		{NULL, NULL}
	};
}

Kylin::Character::Character()
: m_pActDispatcher(NULL)
, m_pAvatar(NULL)
{

}

KVOID Kylin::Character::OnEntityCllsn( Entity* pCollidee,const KPoint3& rNormal )
{
	int i = 0;
}

KBOOL Kylin::Character::OnShouldCllsn( Entity* pCollidee )
{
	if (this != pCollidee)
		return true;

	return false;
}

KVOID Kylin::Character::Tick( KFLOAT fElapsed )
{
	Entity::Tick(fElapsed);

	SAFE_CALL(m_pActDispatcher,Tick(fElapsed));
}

KBOOL Kylin::Character::Init( const PropertySet& kProp )
{
	if (!Entity::Init(kProp))
		return false;
	
	this->GetEntityPtr()->setQueryFlags(KylinRoot::KR_NPC_MASK);
	//////////////////////////////////////////////////////////////////////////
	//SetTranslateToTerrain(this->GetTranslate());
	m_pAvatar = KNEW Avatar(this);
	//////////////////////////////////////////////////////////////////////////
	//
	m_pActDispatcher = KNEW ActionDispatcher(this->GetID());

	return true;
}


KVOID Kylin::Character::PostSpawn()
{
	Entity::PostSpawn();

	//////////////////////////////////////////////////////////////////////////
	KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"do_spawn");

}

KVOID Kylin::Character::SetActionFactory( ActionFactory* pActFactory )
{
	assert(pActFactory);
	
	SAFE_CALL(m_pActDispatcher,SetFactory(pActFactory));
}

KVOID Kylin::Character::Destroy()
{
	m_pActDispatcher->DestroyAllAction();
	SAFE_DEL(m_pActDispatcher);
	
	SAFE_DEL(m_pAvatar);

	Entity::Destroy();
}

Kylin::ActionDispatcher* Kylin::Character::GetActionDispatcher()
{
	return m_pActDispatcher;
}

Kylin::Avatar* Kylin::Character::GetAvatar()
{
	return m_pAvatar;
}
