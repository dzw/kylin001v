#pragma once

#include "Level.h"

namespace Kylin
{

	class ClLevel : public Level
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(ClLevel, Entity)
		Declare_Entity_Creator(ClLevel)

	protected:
		KVOID	EV_DoQuit(EventPtr spEV);
	};
}