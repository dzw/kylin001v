#include "cltpch.h"
#include "SummonSkill.h"
#include "ActionDispatcher.h"
#include "KylinRoot.h"
#include "AnimationProxy.h"
#include "Entity.h"


namespace Kylin
{
	SummonSkill::SummonSkill(ActionDispatcher* pDispatcher)
		: Action(pDispatcher)
	{

	}

	KBOOL SummonSkill::Init( const PropertySet& kProp )
	{
		if (!Action::Init(kProp))
			return false;

		return true;
	}

	KVOID SummonSkill::Tick( KFLOAT fElapsed )
	{
		Action::Tick(fElapsed);

	}

	KVOID SummonSkill::Destroy()
	{

		//////////////////////////////////////////////////////////////////////////
		Action::Destroy();
	}

	Factor* SummonSkill::SpawnFactor()
	{
		//-----------------------------------------------------------
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
		//-----------------------------------------------------------
		// spawn npc object
// 		Kylin::Entity* pObj = KylinRoot::getSingletonPtr()->SpawnCharactor(1,id_npc);
// 		if (BtIsKindof(pObj,NpcObject))
// 		{
// 			NpcObject* pNpc = BtStaticCast(NpcObject,pObj);
// 			pNpc->SetAIHandler()
// 		}

		return NULL;
	}

	KVOID SummonSkill::OnTriggered( Factor* pFactor )
	{

	}
}