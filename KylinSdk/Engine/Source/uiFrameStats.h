#pragma once

#include "GuiManager.h"

namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	class DebugFrameStats : public GuiBase
	{
	public:
		DebugFrameStats();
		virtual ~DebugFrameStats();

	public:
		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);

	protected:

	private:
		MyGUI::StaticTextPtr	m_pFrameStats;
	};
}