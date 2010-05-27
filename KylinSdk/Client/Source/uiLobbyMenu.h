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
		virtual KVOID SetWidgetEnable(KSTR sName, KBOOL bEnable);

		void NotifyButtonClick(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnStart, "_Main");
		MyGUI::Button* m_pBtnStart;
		ATTRIBUTE_FIELD_WIDGET_NAME(LobbyMenu, m_pBtnQuit, "Quit");
		MyGUI::Button* m_pBtnQuit;
	};
}