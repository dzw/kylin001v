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
	
	//////////////////////////////////////////////////////////////////////////
	// test
	EventPtr spEV(
		new Event(
		&ev_post_destroy, 
		Event::ev_timing, 
		5.0f, 
		0, 
		NULL
		));

	KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);

	return true;
}

KVOID Kylin::Factor::PostDestroy()
{
	//////////////////////////////////////////////////////////////////////////
	// test code
	SAFE_CALL(m_spHostAct,OnTriggered(this));
	
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

