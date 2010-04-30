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

		KVOID Tick(KFLOAT fElapsed);

		LevelController* GetController();

	protected:
		void EV_LVLCommonEvent(EventPtr ev);
		void EV_LVLInit(EventPtr ev);
		void ExecuteScriptFunc(Kylin::Entity * pEnt, const EventTemplate * pTemplate, KCCHAR * sig, ...);

	protected:
		LevelController* m_pController;
	};
};

