#include "corepch.h"
#include "PetAI.h"
#include "KylinRoot.h"
#include "Character.h"
#include "randomc.h"
#include "NpcObject.h"


Kylin::PetAI::PetAI( NpcObject* pHost )
: BaseAI(pHost)
{

}

Kylin::PetAI::~PetAI()
{

}

KBOOL Kylin::PetAI::Init()
{
	if (!BaseAI::Init())
		return false;
	
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_pHostChar->GetID());
	if (BtIsKindOf(NpcObject,pEnt))
	{
		NpcObject* pNpc = BtStaticCast(NpcObject,pEnt);
		Enter_Follow(pNpc->GetMasterWorldID());
	}

	return true;
}

KVOID Kylin::PetAI::Tick( KFLOAT fElapsed )
{
	BaseAI::Tick(fElapsed);
}


RC_RESULT Kylin::PetAI::Enter_Follow( KUINT uTargetObj )
{
	KFLOAT fX = .0f;
	KFLOAT fZ = .0f;
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(uTargetObj);
	if (BtIsKindOf(Character,pEnt))
	{
		fX = pEnt->GetTranslate().x-1;
		fZ = pEnt->GetTranslate().z-1;
	}
	else
		return RC_SKIP;

	return Enter_Move(fX,fZ);
}

RC_RESULT Kylin::PetAI::Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KFLOAT fDir )
{
	
	return RC_SKIP;
}


KBOOL Kylin::PetAI::Tick_UseSkill( KFLOAT fElapsed )
{
	return true;
}


KBOOL Kylin::PetAI::Tick_Idle( KFLOAT fElapsed )
{
	if (m_fStayTime > 0)
	{
		m_fStayTime-=fElapsed;
		if (m_fStayTime < 0)
		{
			Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_pHostChar->GetID());
			if (BtIsKindOf(NpcObject,pEnt))
			{
				NpcObject* pNpc = BtStaticCast(NpcObject,pEnt);
				Enter_Follow(pNpc->GetMasterWorldID());
			}

			m_fStayTime = m_pRandomGenerator->Random();
		}
	}

	return true;
}