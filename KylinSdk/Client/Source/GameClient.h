#pragma once

#include "GameFrame.h"

namespace Kylin
{
	class GameClient : public GameFrame
	{
	public:
		
		virtual KVOID	Entrance(KCHAR *argv);
		virtual KVOID 	DataTableLoading();

	protected:
		virtual KVOID	OpenScriptBinding(lua_State *L);
	};
}