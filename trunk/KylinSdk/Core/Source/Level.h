#pragma once

#include "Entity.h"
#include "core.h"
#include "LevelController.h"

namespace Kylin
{
	
	class Level : public Entity
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(Level, Entity)
		Declare_Entity_Creator(Level)

		Level(KVOID);
		virtual ~Level(KVOID);
			
		virtual	KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID OnTimer();
		virtual KVOID SetTimer(KFLOAT fTimeStep);
		virtual KVOID KillTimer();

	protected:
		KBOOL	m_bTimerEnable;
		KFLOAT	m_fInitTimeStep;
		KFLOAT	m_fTimeStep;
	};
};

