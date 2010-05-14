#pragma once

#include "GameFrame.h"

namespace Kylin
{
	class GameClient : public GameFrame
	{
	public:
		
		virtual KVOID	Entrance();

	protected:
		virtual KVOID	Destroy();
	};
}