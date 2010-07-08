#include "cltpch.h"
#include "ActSkill.h"
#include "Factor.h"
#include "ActionDispatcher.h"
#include "registerclass.h"
#include "KylinRoot.h"
#include "rOgreUtils.h"
#include "Entity.h"
#include "AnimationProxy.h"
#include "RemoteEvents.h"


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
	// 播放角色动画
	KSTR sAnim;
	if (m_kProperty.GetStrValue("$Animation",sAnim))
	{
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_pDispatcher->GetHostWorldID());
		if (pEnt)
		{
			pEnt->GetAnimationProxy()->Play(sAnim);
		}
	}
	
	//-----------------------------------------------------
	return Action::SpawnFactor();
}

KVOID Kylin::ActSkill::OnTriggered( Factor* pFactor )
{
	KPoint3 kPos = pFactor->GetTranslate();
	KFLOAT fRadius = 5.0f;
	KVEC<Ogre::Entity*> kEnts;

	OgreUtils::SphereQuery(kPos,fRadius,kEnts,KylinRoot::KR_NPC_MASK);
	
	for (KUINT i =0; i < kEnts.size(); i++)
	{
		if (kEnts[i]->getUserAny().isEmpty())
			continue;

		KUINT uID = Ogre::any_cast<KUINT>(kEnts[i]->getUserAny());
		Kylin::Entity* pTarget = KylinRoot::GetSingletonPtr()->GetEntity(uID);
		if (pTarget)
		{
			// 发送伤害消息
			EventPtr spEV(
				new Event(
				&ev_post_damage, 
				Event::ev_immediate, 
				0, 
				0, 
				NULL
				));

			KylinRoot::GetSingletonPtr()->PostMessage(uID,spEV);
		}
	}

}