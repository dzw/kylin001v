#include "CorePCH.h"
#include "Level.h"
#include "RegisterClass.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "ScriptVM.h"
#include "RemoteEvents.h"


namespace Kylin
{
	BtImplementRTTI(Level, Entity, id_level);

	Implement_Event_Handler(Level, Entity)
	{
		{&ev_on_timer,				&EV_OnTimer},
		{&ev_do_quit,				&EV_DoQuit},
		{NULL, NULL}
	};

	
	Level::Level(KVOID)
		: m_fTimeStep(.0f)
		, m_bTimerEnable(false)
		, m_nSuccessFactors(0)
		, m_nFailureFactors(0)
	{
		
	}

	Level::~Level(KVOID)
	{
		
	}

	KBOOL Level::Init( const PropertySet& kProp )
	{
		if (!Entity::Init(kProp))
			return false;
		
		KSTR sName;
		if ( !m_kProperty.GetStrValue("$Name",sName) )
			return false;
		//-----------------------------------------------------------------
		//
		KVEC<KCCHAR *> kModules;
		kModules.push_back(sName.data());
		kModules.push_back("lvl");

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"init",true,"i",GetID());
		//-----------------------------------------------------------------

		return true;
	}

	KVOID Level::PostDestroy()
	{
		KSTR sName;
		m_kProperty.GetStrValue("$Name",sName);
		//-----------------------------------------------------------------
		//
		KVEC<KCCHAR *> kModules;
		kModules.push_back(sName.data());
		kModules.push_back("lvl");

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"destroy",true,"i",GetID());
		//-----------------------------------------------------------------

		Entity::PostDestroy();
	}

	KVOID Level::OnTimer()
	{
		KSTR sName;
		m_kProperty.GetStrValue("$Name",sName);
		//-----------------------------------------------------------------
		// 调用脚步计时器
		KVEC<KCCHAR *> kModules;
		kModules.push_back(sName.data());
		kModules.push_back("lvl");

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"on_timer",true,"i",GetID());

		//-----------------------------------------------------------------
		// 发送消息
		EventPtr spEV(
			KNEW Event(
			&ev_on_timer, 
			Event::ev_timing, 
			m_fTimeStep,
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
	}

	KVOID Level::SetTimer( KFLOAT fTimeStep )
	{
		m_fTimeStep		= fTimeStep;
		m_bTimerEnable	= true;

		OnTimer();
	}

	KVOID Level::KillTimer()
	{
		m_bTimerEnable = false;
	}

	KVOID Level::EV_OnTimer( EventPtr spEV )
	{
		if (m_bTimerEnable)
		{
			OnTimer();
		}
	}

	KVOID Level::EV_DoQuit( EventPtr spEV )
	{
		OgreRoot::GetSingletonPtr()->ShutDown();
	}

	KVOID Level::AddSuccessFactor()
	{
		m_nSuccessFactors--;
		if (m_nSuccessFactors <= 0)
		{
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_success");
		}
	}

	KVOID Level::AddFailureFactor()
	{
		m_nFailureFactors--;
		if (m_nFailureFactors <= 0)
		{
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_failure");
		}
	}
	// 	KVOID Level::SetVictoryFactors( KINT nCount )
// 	{
// 		assert(nCount > 0);
// 		m_nVictoryFactors = nCount;
// 	}
// 
// 	KVOID Level::AddVictoryFactor()
// 	{
// 		m_nVictoryFactors--;
// 		if (m_nVictoryFactors <= 0)
// 		{
// 			DoTotalling();
// 		}
// 	}
// 
// 	KVOID Level::DoTotalling()
// 	{
// 		//-----------------------------------------------------------------
// 		// 发送消息
// 		EventPtr spEV(
// 			KNEW Event(
// 			&ev_sync_gameresult, 
// 			Event::ev_immediate, 
// 			0,
// 			1, 
// 			EventArg(m_nVictoryFactors <= 0)
// 			));
// 
// 		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
// 	}
}

