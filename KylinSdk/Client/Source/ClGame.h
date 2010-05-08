#pragma once

#include "GameStatus.h"
#include "GuiManager.h"

namespace Kylin
{
	class ClGame : public GSGame , public GuiShell
	{
	public:
		ClGame();

		virtual KBOOL Initialize();
		virtual KVOID Destroy();
		
		virtual KVOID UiLoader();

	protected:
		
	};
}