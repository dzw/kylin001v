#include "CorePCH.h"
#include "Level.h"
#include "RegisterClass.h"
#include "LevelController.h"


namespace Kylin
{
	BtImplementRTTI(Level, Entity, id_level);

	Implement_Event_Handler(Level, Entity)
	{
		{NULL, NULL}
	};

	
	Level::Level(KVOID)
	{
		//SetEntityFlag(c_flagTick);
		m_pController = KNEW LevelController;
	}

	Level::~Level(KVOID)
	{
		SAFE_DEL(m_pController);
	}

	KVOID Level::Tick(KFLOAT fElapsed)
	{
		m_pController->Tick(fElapsed);
	}


	KVOID Level::EV_LVLInit(EventPtr ev)
	{
		m_pController->EV_LVLInit(ev);
	}

	//common event handler, which is event without params
	KVOID Level::EV_LVLCommonEvent(EventPtr ev)
	{
		m_pController->EV_LVLCommonEvent(ev);
	}

	KVOID Level::ExecuteScriptFunc(Entity * pEnt, const EventTemplate * pTemplate, const char * sig, ...)
	{
		va_list vl;
		va_start(vl, pTemplate);
		m_pController->ExecuteScriptFunc(pEnt, pTemplate, sig, vl);
		va_end(vl);
	}

	LevelController* Level::GetController()
	{
		return m_pController;
	}
}

