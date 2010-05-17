#pragma once

#include "GameFrame.h"

namespace Kylin
{
	class GameClient : public GameFrame
	{
	public:
		
		virtual KVOID	Entrance();
		virtual KVOID 	DataTableLoading();

	protected:
		virtual KVOID	Destroy();
	};
}