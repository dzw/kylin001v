#pragma once

#include "GameStatus.h"
#include "GuiManager.h"

namespace Kylin
{
	class ClGame : public GSGame , public GuiShell
	{
	public:
		ClGame();

		virtual KBOOL		Initialize();
		virtual KVOID		Destroy();
		virtual KVOID		Tick(KFLOAT fElapsed);

		virtual KVOID		UiLoader();
		
		virtual KVOID		Quit() { m_bQuitFlag = true; }

	protected:
		KBOOL m_bQuitFlag;
	};
}