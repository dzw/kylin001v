#include "cltpch.h"
#include "BulletFactor.h"
#include "clRegisterClass.h"
#include "DataManager.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"


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
		Factor::PostSpawn();

	}

	KVOID BulletFactor::PostDestroy()
	{
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Reject(this);

		Factor::PostDestroy();
	}

	KVOID BulletFactor::Moving( KFLOAT fElapsed )
	{
		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(this,KPoint3(0, 0, m_fVelocity),1.0f);
	}

	KVOID BulletFactor::EV_PostTouchdown( EventPtr spEV )
	{
		EventPtr spPosEV(
			new Event(
			&ev_post_destroy, 
			Event::ev_nextframe, 
			0, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spPosEV);
	}
}

