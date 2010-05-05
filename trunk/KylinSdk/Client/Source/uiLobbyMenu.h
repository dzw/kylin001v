#pragma once

#include "GuiManager.h"
#include "BaseLayout/BaseLayout.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(LobbyMenu, "lobby.layout");
	class LobbyMenu : public GuiBase
					, public wraps::BaseLayout
	{
	public:


	public:
		LobbyMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID SetVisible(KBOOL bVisible);
		
		void NotifyButtonClick(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnStart, "_Main");
		MyGUI::Button* m_pBtnStart;
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnQuit, "Quit");
		MyGUI::Button* m_pBtnQuit;
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnNew, "New");
		MyGUI::Button* m_pBtnNew;
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnNextChar, "Next");
		MyGUI::Button* m_pBtnNextChar;

	};
}