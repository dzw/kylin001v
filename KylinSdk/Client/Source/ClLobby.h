#pragma once

#include "GameStatus.h"
#include "GuiManager.h"

namespace Kylin
{	
	class LobbyScene;
	class ClLobby : public GameStatus , public GuiShell
	{
	public:
		ClLobby();

		virtual KBOOL Initialize();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID UiLoader();
		
		LobbyScene*	  GetLobbyScene();

	protected:
		LobbyScene* m_pLobbyScene;
	};
}