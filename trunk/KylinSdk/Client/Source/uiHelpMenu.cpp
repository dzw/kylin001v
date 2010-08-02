#include "cltpch.h"
#include "uiHelpMenu.h"	



Kylin::HelpMenu::HelpMenu()
: GuiBase(CLASS_TO(HelpMenu))
{
	initialiseByAttributes(this);

	SetVisible(false);
}

KBOOL Kylin::HelpMenu::Initialize()
{
	MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(mMainWidget);
	window->eventWindowButtonPressed		= MyGUI::newDelegate(this, &Kylin::HelpMenu::notifyWindowPressed);

	return true;
}

KVOID Kylin::HelpMenu::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	mMainWidget->setVisible(bVisible);

}

KVOID Kylin::HelpMenu::notifyWindowPressed( MyGUI::WidgetPtr _widget, const std::string& _name )
{
	MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(_widget);
	if (_name == "close") 
	{
		SetVisible(false);
	}
}

KVOID Kylin::HelpMenu::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::H)
	{
		if (IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}