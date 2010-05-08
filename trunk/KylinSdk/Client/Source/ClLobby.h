#pragma once

#include "GameStatus.h"
#include "GuiManager.h"

namespace Kylin
{	
	class Stage;
	class ClLobby : public GameStatus , public GuiShell
	{
	public:
		ClLobby();

		virtual KBOOL Initialize();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID UiLoader();

	protected:
		Stage* m_pStage;
	};
}