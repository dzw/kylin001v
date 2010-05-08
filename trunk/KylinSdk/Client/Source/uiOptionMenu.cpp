#include "cltpch.h"
#include "uiOptionMenu.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "GameStatus.h"
#include "ClLobby.h"


Kylin::OptionMenu::OptionMenu()
{
	initialiseByAttributes(this);

	m_sGuiName = "OptionMenu";

	SetVisible(false);
}

KBOOL Kylin::OptionMenu::Initialize()
{
	m_pBtnResume->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnQuit->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);
	m_pBtnExit->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::OptionMenu::NotifyButtonClick);

	return true;
}

KVOID Kylin::OptionMenu::Render( KFLOAT fElapsed )
{

}

KVOID Kylin::OptionMenu::Destroy()
{
	

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

}

KVOID Kylin::OptionMenu::OnKeyDown(KUINT uKey)
{

	if (uKey == MyGUI::KeyCode::Escape)
	{
		if (IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}