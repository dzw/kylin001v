#include "corepch.h"
#include "Factor.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "Action.h"


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
{

}


KBOOL Kylin::Factor::Init( const PropertySet& kProp )
{
	if (!Kylin::Entity::Init(kProp))
		return false;
	

	//------------------------------------------------------------------
	// �������ӵĴ���ʱ��
	KFLOAT fTimes = .0f;
	if (m_kProperty.GetFloatValue("$Times",fTimes))
	{
		EventPtr spEV(
			new Event(
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
	// �ָ���Ч
	KSTR sEffect;
	if (m_kProperty.GetStrValue("$Effect",sEffect))
		ActivateEffect(sEffect,false);

	//////////////////////////////////////////////////////////////////////////
	// ������Ϣ
	SAFE_CALL(m_spHostAct,OnTriggered(this));
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
	// ������Ч
	KSTR sEffect;
	if (m_kProperty.GetStrValue("$Effect",sEffect))
		ActivateEffect(sEffect,true);
}
