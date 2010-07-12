#include "corepch.h"
#include "Factor.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "Action.h"
#include "EffectManager.h"


namespace Kylin
{
	BtImplementRTTI(Factor, Entity, id_factor);

	Implement_Event_Handler(Factor, Entity)
	{
		{NULL, NULL}
	};
}

Kylin::Factor::Factor()
: m_spHostAct(NULL)
, m_bNeedCallback(true)
{

}


KBOOL Kylin::Factor::Init( const PropertySet& kProp )
{
	if (!Kylin::Entity::Init(kProp))
		return false;
	

	//------------------------------------------------------------------
	// 设置因子的存在时间
	KFLOAT fTimes = .0f;
	if (m_kProperty.GetFloatValue("$Times",fTimes))
	{
		EventPtr spEV(
			KNEW Event(
			&ev_post_destroy, 
			Event::ev_timing, 
			fTimes, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
	}
	//------------------------------------------------------------------

	return true;
}

KVOID Kylin::Factor::PostDestroy()
{
	// 恢复特效
	KSTR sEffect;
	if (m_kProperty.GetStrValue("$DestroyEffect",sEffect))
		ActivateEffect(sEffect,false);

	//////////////////////////////////////////////////////////////////////////
	// 反馈信息
	if (m_bNeedCallback)
	{
		SAFE_CALL(m_spHostAct,OnTriggered(this));
	}
	
	SAFE_CALL(m_spHostAct,RemoveFactor(this->GetID()));

	Entity::PostDestroy();
}

KVOID Kylin::Factor::Tick( KFLOAT fElapsed )
{
	Kylin::Entity::Tick(fElapsed);

}


KVOID Kylin::Factor::SetHostAction( Action* pAct )
{
	m_spHostAct = pAct;
}

KVOID Kylin::Factor::PostSpawn()
{
	// 激活特效
	KSTR sEffect;
	if (m_kProperty.GetStrValue("$SpawnEffect",sEffect))
		ActivateEffect(sEffect,true);
}

KVOID Kylin::Factor::BindEffect( PropertySet kProp )
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
		}
	}
}

KVOID Kylin::Factor::SetTarget( KUINT uTarget )
{
	m_kProperty.SetValue("$TargetFoe",uTarget);
}

KUINT Kylin::Factor::GetTarget()
{
	KUINT uTargetFoe = INVALID_ID;
	
	m_kProperty.GetUIntValue("$TargetFoe",uTargetFoe);

	return uTargetFoe;
}