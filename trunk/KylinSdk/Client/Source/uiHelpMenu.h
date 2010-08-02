#pragma once

#include "GuiManager.h"
#include "BaseLayout/BaseLayout.h"
#include "InputListener.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(HelpMenu, "help.layout");
	class HelpMenu : public GuiBase
		, public wraps::BaseLayout
		, public InputListener
	{
	public:
		HelpMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed) {}
		virtual KVOID Destroy() {}

		virtual KVOID SetVisible(KBOOL bVisible);

	protected:
		virtual KVOID notifyWindowPressed(MyGUI::WidgetPtr _widget, const std::string& _name);
		virtual KVOID OnKeyDown(KUINT uKey);

	};

}