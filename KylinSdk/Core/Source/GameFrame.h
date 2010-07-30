#pragma once

#include "core.h"
#include "AppFrame.h"

namespace Kylin
{
	class GameFrame : public AppFrame
	{
	public:  
		GameFrame();
		virtual ~GameFrame();
		
		virtual SceneLoader*	CreateSceneLoader() = 0;
		virtual KVOID			CreateWidgets();
		// Clear out everything and reset variables to start the game over again without leaving the state
		virtual KVOID			Run();
		virtual KVOID			Entrance(KCHAR *argv) = 0;
		virtual KVOID			OnIdle(KFLOAT fElapsed);
		
		virtual KVOID			Destroy();
		virtual KVOID			SwitchStatus( GameStatus* pStatus );

	protected:
		virtual KVOID			OpenScriptBinding(lua_State *L);

	private:
		friend class KylinRoot;
		
		GameStatus*	m_pActiveStatus;
		
	};
}

