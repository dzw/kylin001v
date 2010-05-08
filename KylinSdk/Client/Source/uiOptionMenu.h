#pragma once

#include "GuiManager.h"
#include "BaseLayout/BaseLayout.h"
#include "InputListener.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(OptionMenu, "option.layout");
	class OptionMenu : public GuiBase
					, public wraps::BaseLayout
					, public InputListener
	{
	public:
		OptionMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID SetVisible(KBOOL bVisible);
		
		void NotifyButtonClick(MyGUI::Widget* _sender);

	protected:
		virtual KVOID OnKeyDown(KUINT uKey);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(OptionMenu, m_pBtnQuit, "quit_game");
		MyGUI::Button* m_pBtnQuit;
		ATTRIBUTE_FIELD_WIDGET_NAME(OptionMenu, m_pBtnResume, "resume_game");
		MyGUI::Button* m_pBtnResume;
		ATTRIBUTE_FIELD_WIDGET_NAME(OptionMenu, m_pBtnExit, "exit");
		MyGUI::Button* m_pBtnExit;

	};
}