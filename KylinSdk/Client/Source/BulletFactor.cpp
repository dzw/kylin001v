#include "cltpch.h"
#include "BulletFactor.h"
#include "clRegisterClass.h"
#include "DataManager.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "EffectManager.h"
#include "Action.h"


namespace Kylin
{
	BtImplementRTTI(BulletFactor, Factor, id_bullet_factor);

	Implement_Event_Handler(BulletFactor, Factor)
	{
		{&ev_post_touchdown,			&EV_PostTouchdown},
		{NULL, NULL}
	};

	BulletFactor::BulletFactor()
		: m_fVelocity(.0f)
		, m_bExplode(false)
	{

	}

	KBOOL BulletFactor::Init( const PropertySet& kProp )
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

	KVOID BulletFactor::Tick( KFLOAT fElapsed )
	{
		Factor::Tick(fElapsed);

		Moving(fElapsed);
	}

	KVOID BulletFactor::PostSpawn()
	{
		//Factor::PostSpawn();
		// 激活特效
		KSTR sEffect;
		if (m_kProperty.GetStrValue("$SpawnEffect",sEffect))
			ActivateEffect(sEffect,true);
	}

	KVOID BulletFactor::PostDestroy()
	{
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Erase(this);

		Factor::PostDestroy();
	}

	KVOID BulletFactor::Moving( KFLOAT fElapsed )
	{
		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(this,KPoint3(0, 0, m_fVelocity),1.0f);
	}

	KVOID BulletFactor::EV_PostTouchdown( EventPtr spEV )
	{
		OnExplode();
	}

	KVOID BulletFactor::EndTime( KCSTR& sClass,KCSTR& sName, KANY aUserData )
	{
		if (sClass == CLASS_TO(EffectParticle))
		{
			ActivateEffect(sName,false);

			KINT nData = boost::any_cast<KINT>(aUserData);
			if (nData == 1)
			{
				KSTR sEffect;
				if (m_kProperty.GetStrValue("$IdleEffect",sEffect))
					ActivateEffect(sEffect,true);
			}
			else if (nData == 3)
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

	KVOID BulletFactor::BindEffect( PropertySet kProp )
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
		if (kProp.GetStrValue("$IdleEffect",sEffect))
		{
			KFLOAT fTimes = -1.0f;
			kProp.GetFloatValue("$IdleEffectTime",fTimes);

			EffectObject* pObj = AttachEffect(sEffect,fTimes);
			if (pObj)
			{	// 设置特效缩放
				KFLOAT fEfScale;
				if (kProp.GetFloatValue("$IdleEffectScale",fEfScale))
				{
					pObj->SetScale(fEfScale);
				}
				pObj->SetUserData(2);
				pObj->SetCallbackObj(this);
			}
		}
		if (kProp.GetStrValue("$DestroyEffect",sEffect))
		{
			KFLOAT fTimes = -1.0f;
			kProp.GetFloatValue("$DestroyEffectTime",fTimes);

			EffectObject* pObj = AttachEffect(sEffect,fTimes);
			if (pObj)
			{	// 设置特效缩放
				KFLOAT fEfScale;
				if (kProp.GetFloatValue("$DestroyEffectScale",fEfScale))
				{
					pObj->SetScale(fEfScale);
				}

				pObj->SetUserData(3);
				pObj->SetCallbackObj(this);
			}
		}
	}

	KVOID BulletFactor::SetupCllsn( PropertySet kProp )
	{
		PhyX::CollisionMonitor::CollisionData* pData = PhyX::PhysicalSystem::GetSingletonPtr()->GetCollisionMonitor()->Commit(this,true);
		//pData->SetCallbackFunc(&CollideCallback);
		pData->SetEnable(true);
	}

	KBOOL BulletFactor::OnShouldCllsn( Kylin::Entity* pCollidee )
	{
		Kylin::Entity* pHost = KylinRoot::GetSingletonPtr()->GetEntity(m_spHostAct->GetHostWorldID());
		
		// 被碰撞者是武器发射者不做碰撞
		if (pHost && pHost == pCollidee)
			return false;
		
		// 爆炸后不做碰撞
		if (m_bExplode)
			return false;

		return true;
	}

	KVOID BulletFactor::OnEntityCllsn( Kylin::Entity* pCollidee,const KPoint3& rNormal )
	{
		PostDamage(pCollidee);
		OnExplode();
	}

	KVOID BulletFactor::OnExplode()
	{
		if (!m_bExplode)
		{
			KSTR sEffect;
			if (m_kProperty.GetStrValue("$DestroyEffect",sEffect))
				ActivateEffect(sEffect,true);

			if (m_kProperty.GetStrValue("$IdleEffect",sEffect))
				ActivateEffect(sEffect,false);

			// 接触地面后速度为0
			m_fVelocity		= .0f;
			m_bExplode		= true;
			m_bNeedCallback = false;

			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Erase(this);
		}
	}

	KVOID BulletFactor::PostDamage( Entity* pCollidee )
	{
		if (!m_bExplode)
		{
			EventPtr spEV(
				KNEW Event(
				&ev_post_damage, 
				Event::ev_immediate, 
				0, 
				3, 
				EventArg(m_spHostAct->GetHostWorldID()),
				EventArg(m_spHostAct->GetMinDamage()),
				EventArg(m_spHostAct->GetMaxDamage())
				));

			KylinRoot::GetSingletonPtr()->PostMessage(pCollidee->GetID(),spEV);
		}
	}
}

