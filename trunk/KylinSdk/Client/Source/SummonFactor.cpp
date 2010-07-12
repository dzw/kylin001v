#include "cltpch.h"
#include "SummonFactor.h"
#include "clRegisterClass.h"
#include "DataManager.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "EffectManager.h"
#include "clRegisterClass.h"
#include "NpcObject.h"
#include "Action.h"
#include "PetAI.h"



namespace Kylin
{
	BtImplementRTTI(SummonFactor, Factor, id_summon_factor);

	Implement_Event_Handler(SummonFactor, Factor)
	{
		{NULL, NULL}
	};

	SummonFactor::SummonFactor()
		: m_fVelocity(.0f)
	{

	}

	KBOOL SummonFactor::Init( const PropertySet& kProp )
	{
		if ( !Factor::Init(kProp) )
			return false;
		
		//-----------------------------------------------------------
		KUINT uFactorGId = -1;
		if ( !m_kProperty.GetUIntValue("$GID",uFactorGId) )
			return false;

		KANY aV;
		if ( DataManager::GetSingletonPtr()->Select("FACTOR_DB",uFactorGId,"VELOCITY",aV) )
		{
			m_fVelocity = boost::any_cast<KFLOAT>(aV);
		}
		//-----------------------------------------------------------
		
		return true;
	}

	KVOID SummonFactor::Tick( KFLOAT fElapsed )
	{
		Factor::Tick(fElapsed);

		Moving(fElapsed);
	}

	KVOID SummonFactor::PostSpawn()
	{
		//Factor::PostSpawn();
		// 激活特效
		KSTR sEffect;
		if (m_kProperty.GetStrValue("$SpawnEffect",sEffect))
			ActivateEffect(sEffect,true);
	}

	KVOID SummonFactor::PostDestroy()
	{
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Erase(this);
		
		// 特效播放完毕后调用
		DoSummon();

		Factor::PostDestroy();
	}

	KVOID SummonFactor::Moving( KFLOAT fElapsed )
	{
		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(this,KPoint3(0, 0, m_fVelocity),1.0f);
	}


	KVOID SummonFactor::EndTime( KCSTR& sClass,KCSTR& sName, KANY aUserData )
	{
		if (sClass == CLASS_TO(EffectParticle))
		{
			ActivateEffect(sName,false);

			KINT nData = boost::any_cast<KINT>(aUserData);
			if (nData == 1)
			{
				EventPtr spPosEV(
					KNEW Event(
					&ev_post_destroy, 
					Event::ev_nextframe,
					0, 
					0, 
					NULL
					));

				KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spPosEV);
			}
		}
	}

	KVOID SummonFactor::BindEffect( PropertySet kProp )
	{
		//////////////////////////////////////////////////////////////////////////
		// 加载特效
		KSTR sEffect;
		if (kProp.GetStrValue("$SpawnEffect",sEffect))
		{
			KFLOAT fTimes = -1.0f;
			kProp.GetFloatValue("$SpawnEffectTime",fTimes);

			EffectObject* pObj = AttachEffect(sEffect,fTimes);
			if (pObj)
			{	// 设置特效缩放
				KFLOAT fEfScale;
				if (kProp.GetFloatValue("$SpawnEffectScale",fEfScale))
				{
					pObj->SetScale(fEfScale);
				}
				pObj->SetUserData(1);
				pObj->SetCallbackObj(this);
			}
		}

	}

	KVOID SummonFactor::DoSummon()
	{
		KUINT uFactorGId = -1;
		if ( !m_kProperty.GetUIntValue("$GID",uFactorGId) )
			return;

		KANY aV;
		if ( DataManager::GetSingletonPtr()->Select("FACTOR_DB",uFactorGId,"WARD",aV) )
		{
			KUINT uCharID = boost::any_cast<KINT>(aV);

			Kylin::Entity* pNpc = KylinRoot::GetSingletonPtr()->SpawnCharactor(uCharID,id_npc);
			if (BtIsKindOf(NpcObject,pNpc))
			{
				NpcObject* pNpcObj = BtStaticCast(NpcObject,pNpc);

				Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_spHostAct->GetHostWorldID());
				if (BtIsKindOf(Character,pEnt))
				{
					Character* pChar = BtStaticCast(Character,pEnt);
					pNpcObj->SetTranslate(this->GetTranslate());
					pNpcObj->SetMaster(pChar);
					pNpcObj->SetAIHandler(KNEW PetAI(pNpcObj));
				}

				
			}
		}
	}
}

