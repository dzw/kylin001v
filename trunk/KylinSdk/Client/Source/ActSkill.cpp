#include "cltpch.h"
#include "ActSkill.h"
#include "Factor.h"
#include "ActionDispatcher.h"
#include "registerclass.h"
#include "KylinRoot.h"
#include "rOgreUtils.h"
#include "Entity.h"
#include "RemoteEvents.h"
#include "DataManager.h"


Kylin::ActSkill::ActSkill( ActionDispatcher* pDispatcher )
: Action(pDispatcher)
{

}


KVOID Kylin::ActSkill::OnTriggered( Factor* pFactor )
{
	// 只有以位置的技能是范围处理
	KFLOAT fRadius = GetRange();
	assert(fRadius);

	KPoint3 kPos = pFactor->GetTranslate();
	
	KVEC<Ogre::Entity*> kEnts;
	
	if ( m_eType == AT_TAR)	
		HitTarget(pFactor->GetTarget());
	else
		OgreUtils::SphereQuery(kPos,fRadius,kEnts,KylinRoot::KR_NPC_MASK);
	
	for (KUINT i =0; i < kEnts.size(); i++)
	{
		if (kEnts[i]->getUserAny().isEmpty())
			continue;

		KUINT uID = Ogre::any_cast<KUINT>(kEnts[i]->getUserAny());
		if (uID != GetHostWorldID())
			HitTarget(uID);
	}
}


KVOID Kylin::ActSkill::HitTarget( KUINT uID )
{
	Kylin::Entity* pTarget = KylinRoot::GetSingletonPtr()->GetEntity(uID);
	if (pTarget)
	{
		// 发送伤害消息
		EventPtr spEV(
			KNEW Event(
			&ev_post_damage, 
			Event::ev_immediate, 
			0, 
			3, 
			EventArg(GetHostWorldID()),
			EventArg(GetMinDamage()),
			EventArg(GetMaxDamage())
			));

		KylinRoot::GetSingletonPtr()->PostMessage(uID,spEV);
	}
}