#include "cltpch.h"
#include "uiOptionMenu.h"
#include "uiHelpMenu.h"
#include "uiTaskTipsMenu.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "GameStatus.h"
#include "ClLobby.h"


Kylin::OptionMenu::OptionMenu()
: GuiBase(CLASS_TO(OptionMenu))
{
	initialiseByAttributes(this);

	SetVisible(false);
}

KBOOL Kylin::OptionMenu::Initialize()
{
	MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(mMainWidget);
	window->eventWindowButtonPressed		= MyGUI::newDelegate(this, &Kylin::OptionMenu::notifyWindowPressed);
	
	m_pBtnOption->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnHelp->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnResume->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnQuit->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnExit->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);

	return true;
}


KVOID Kylin::OptionMenu::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	mMainWidget->setVisible(bVisible);

}

void Kylin::OptionMenu::NotifyButtonClick( MyGUI::Widget* _sender )
{
	MyGUI::Button* pButton = _sender->castType<MyGUI::Button>();
	if (pButton == m_pBtnQuit)
	{
		SetVisible(false);
		KylinRoot::GetSingletonPtr()->SwitchStatus(KNEW ClLobby());
	}
	else if (pButton == m_pBtnExit) 
	{
		OgreRoot::GetSingletonPtr()->ShutDown();
	}
	else if (pButton == m_pBtnHelp)
	{
		HelpMenu* pMenu = GET_GUI_PTR(HelpMenu);
		pMenu->SetVisible(true);

		SetVisible(false);
	}
}

KVOID Kylin::OptionMenu::OnKeyDown(KUINT uKey)
{

	TaskTipsMenu* pMenu = GET_GUI_PTR(TaskTipsMenu);
	if (pMenu->IsVisible())
		return;

	if (uKey == MyGUI::KeyCode::Escape)
	{
		if (IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}

KVOID Kylin::OptionMenu::notifyWindowPressed( MyGUI::WidgetPtr _widget, const std::string& _name )
{
	MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(_widget);
	if (_name == "close") 
	{
		SetVisible(false);
	}
}