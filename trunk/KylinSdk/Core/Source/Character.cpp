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
#include "Kitbag.h"
#include "DamageSystem.h"


#define CH_HEIGHT 1.0f;

namespace Kylin
{
	BtImplementRTTI(Character, Entity, id_character);

	Implement_Event_Handler(Character, Entity)
	{
		{&ev_post_damage,			&EV_Damage},
		{NULL, NULL}
	};
}

Kylin::Character::Character()
: m_pActDispatcher(NULL)
, m_pAvatar(NULL)
, m_pKitbag(NULL)
{

}

KBOOL Kylin::Character::Init( const PropertySet& kProp )
{
	if (!Entity::Init(kProp))
		return false;
	
	this->GetEntityPtr()->setQueryFlags(KylinRoot::KR_NPC_MASK);
	//////////////////////////////////////////////////////////////////////////
	m_pAvatar = KNEW Avatar(this);
	m_pKitbag = KNEW Kitbag(this);
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

KVOID Kylin::Character::PostDestroy()
{
	m_pActDispatcher->DestroyAllAction();
	SAFE_DEL(m_pActDispatcher);
	
	SAFE_DEL(m_pKitbag);
	SAFE_DEL(m_pAvatar);

	Entity::PostDestroy();
}

KVOID Kylin::Character::Tick( KFLOAT fElapsed )
{
	Entity::Tick(fElapsed);

	SAFE_CALL(m_pActDispatcher,Tick(fElapsed));
}

Kylin::ActionDispatcher* Kylin::Character::GetActionDispatcher()
{
	return m_pActDispatcher;
}

Kylin::Avatar* Kylin::Character::GetAvatar()
{
	return m_pAvatar;
}

KVOID Kylin::Character::EV_Damage( EventPtr spEV )
{
	KUINT uKiller	= boost::get<unsigned int>(spEV->args[0]);
	KINT nMinDamage = boost::get<int>(spEV->args[1]);
	KINT nMaxDamage = boost::get<int>(spEV->args[2]);
	KINT nLevel		= 1;

	Kylin::Entity* pKiller = KylinRoot::GetSingletonPtr()->GetEntity(uKiller);
	if (pKiller)
		pKiller->GetPropertyRef().GetIntValue("$Level",nLevel);

	DamageUnit kUnit(nLevel,nMinDamage,nMaxDamage,1);

	DamageResult kResult = DamageSystem::Calculate(kUnit,this->GetID());
	
	// 执行对应的脚步函数
	KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_damage");
	
	if (kResult.mDIFF <= 0)
	{
		// 执行对应的脚步函数
		KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_killed");
	}
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

Kylin::Kitbag* Kylin::Character::GetKitbag()
{
	return m_pKitbag;
}