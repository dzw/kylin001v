#include "cltpch.h"
#include "ActSkill.h"
#include "Factor.h"
#include "ActionDispatcher.h"
#include "registerclass.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "AnimationProxy.h"


Kylin::ActSkill::ActSkill( ActionDispatcher* pDispatcher )
: Action(pDispatcher)
{

}

KBOOL Kylin::ActSkill::Init( const PropertySet& kProp )
{
	if (!Action::Init(kProp))
		return false;


	return true;
}

KVOID Kylin::ActSkill::Tick( KFLOAT fElapsed )
{
	Action::Tick(fElapsed);


}

KVOID Kylin::ActSkill::Destroy()
{
	
	//////////////////////////////////////////////////////////////////////////
	Action::Destroy();
}

Kylin::Factor* Kylin::ActSkill::SpawnFactor()
{
	//-----------------------------------------------------
	// ²¥·Å½ÇÉ«¶¯»­
	KSTR sAnim;
	if (m_kProperty.GetStrValue("$Animation",sAnim))
	{
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_pDispatcher->GetHostID());
		if (pEnt)
		{
			pEnt->GetAnimationProxy()->Play(sAnim);
		}
	}
	
	//-----------------------------------------------------
	KUINT uFactorGId = 0;
	if (!m_kProperty.GetUIntValue("$FactorID",uFactorGId))
		return NULL;

	//////////////////////////////////////////////////////////////////////////
	// test code
// 	Factor* pFactor = BtStaticCast(Factor,KylinHelper::GetSingletonPtr()->SpawnCharactor(uFactorGId,id_factor));
// 
// 	SAFE_CALL(pFactor,SetHostAction(this));
// 	
// 	return pFactor;

	return Action::SpawnFactor();
}

KVOID Kylin::ActSkill::OnTriggered( Factor* pFactor )
{

}