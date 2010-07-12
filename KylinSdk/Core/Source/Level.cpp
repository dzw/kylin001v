#include "CorePCH.h"
#include "Level.h"
#include "RegisterClass.h"
#include "LevelController.h"
#include "rOgreRoot.h"
#include "ScriptVM.h"


namespace Kylin
{
	BtImplementRTTI(Level, Entity, id_level);

	Implement_Event_Handler(Level, Entity)
	{
		{NULL, NULL}
	};

	
	Level::Level(KVOID)
		: m_fInitTimeStep(.0f)
		, m_bTimerEnable(false)
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

	KVOID Level::Tick(KFLOAT fElapsed)
	{
		if (m_bTimerEnable)
		{
			m_fTimeStep += fElapsed;
			if (m_fTimeStep >= m_fInitTimeStep)
			{
				OnTimer();
				m_fTimeStep = .0f;
			}
		}
	}

	KVOID Level::Destroy()
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
	}

	KVOID Level::OnTimer()
	{
		KSTR sName;
		m_kProperty.GetStrValue("$Name",sName);
		//-----------------------------------------------------------------
		//
		KVEC<KCCHAR *> kModules;
		kModules.push_back(sName.data());
		kModules.push_back("lvl");

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"on_timer",true,"i",GetID());
		//-----------------------------------------------------------------
	}

	KVOID Level::SetTimer( KFLOAT fTimeStep )
	{
		m_fInitTimeStep = fTimeStep;
		m_fTimeStep		= .0f;
		
		m_bTimerEnable	= true;
	}

	KVOID Level::KillTimer()
	{
		m_bTimerEnable = false;
	}
}

