#include "cltpch.h"
#include "uiLobbyMenu.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "GameStatus.h"


Kylin::LobbyMenu::LobbyMenu()
{
	initialiseByAttributes(this);

	m_sGuiName = "LobbyMenu";
}

KBOOL Kylin::LobbyMenu::Initialize()
{
	m_pBtnStart->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::LobbyMenu::NotifyButtonClick);
	m_pBtnQuit->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::LobbyMenu::NotifyButtonClick);
	m_pBtnNew->eventMouseButtonClick		= MyGUI::newDelegate(this, &Kylin::LobbyMenu::NotifyButtonClick);
	m_pBtnNextChar->eventMouseButtonClick	= MyGUI::newDelegate(this, &Kylin::LobbyMenu::NotifyButtonClick);

	return true;
}

KVOID Kylin::LobbyMenu::Render( KFLOAT fElapsed )
{

}

KVOID Kylin::LobbyMenu::Destroy()
{
	

}

KVOID Kylin::LobbyMenu::SetVisible( KBOOL bVisible )
{
	m_pBtnStart->setVisible(bVisible);
	m_pBtnQuit->setVisible(bVisible);
	m_pBtnNew->setVisible(bVisible);
	m_pBtnNextChar->setVisible(bVisible);

}

void Kylin::LobbyMenu::NotifyButtonClick( MyGUI::Widget* _sender )
{
	MyGUI::Button* pButton = _sender->castType<MyGUI::Button>();
	if (pButton == m_pBtnStart)
	{
		SetVisible(false);
		KylinRoot::GetSingletonPtr()->SwitchStatus(KNEW GSGame());
	}
	else if (pButton == m_pBtnQuit) 
	{
		OgreRoot::GetSingletonPtr()->ShutDown();
	}
	else if (pButton == m_pBtnNew) 
	{

	}
	else if (pButton == m_pBtnNextChar) 
	{
		
	}
}
