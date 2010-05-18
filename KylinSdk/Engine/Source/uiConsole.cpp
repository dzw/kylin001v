#include "engpch.h"
#include "uiConsole.h"
#include "CommandHandler.h"

//////////////////////////////////////////////////////////////////////////
Kylin::DebugConsole::DebugConsole()
: m_spContentWidget(NULL)
, m_spCmdWidget(NULL)
, m_pCmdHandler(NULL)
, m_nCurrIndex(0)
{

}

KBOOL Kylin::DebugConsole::Initialize()
{
	m_sGuiName = "DebugConsole";

	//////////////////////////////////////////////////////////////////////////
	//
	const MyGUI::IntSize& rViewSize = MyGUI::Gui::getInstance().getViewSize();
	int nWidth = rViewSize.width;
	int nHeight= rViewSize.height/3;
	m_spContentWidget = MyGUI::Gui::getInstance().createWidget<MyGUI::List>(
		"List", 
		MyGUI::IntCoord(0, 0, nWidth, nHeight), 
		MyGUI::Align::Left, 
		"Main"
		);
	
	//-------------------------------------------------------------------------
	//
	m_spCmdWidget = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>(
		"EditStretch", 
		MyGUI::IntCoord(0, nHeight, nWidth, 28), 
		MyGUI::Align::Left|MyGUI::Align::Bottom, 
		"Main"
		);
	m_spCmdWidget->setCaption("");
	m_spCmdWidget->setEditMultiLine(false);
	//m_spCmdWidget->setNeedMouseFocus(true);
	//////////////////////////////////////////////////////////////////////////
	// Ìí¼ÓÊÂ¼þ
	m_spCmdWidget->eventKeyButtonPressed = newDelegate(this, &Kylin::DebugConsole::notifyButtonPressed);

	SetVisible(false);

	return true;
}

KVOID Kylin::DebugConsole::Render( KFLOAT fElapsed )
{

}

KVOID Kylin::DebugConsole::Destroy()
{
	SAFE_DEL(m_pCmdHandler);

	if (m_spContentWidget)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_spContentWidget);
		m_spContentWidget = NULL;
	}

	if (m_spCmdWidget)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_spCmdWidget);
		m_spCmdWidget = NULL;
	}
}

KVOID Kylin::DebugConsole::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	m_spContentWidget->setVisible(bVisible);
	m_spCmdWidget->setVisible(bVisible);
	m_spCmdWidget->setEnabled(bVisible);
}

KVOID Kylin::DebugConsole::OnRecept()
{

}

void Kylin::DebugConsole::notifyButtonPressed( MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char )
{	
	if (_key == MyGUI::KeyCode::Return)
	{
		KSTR sCmd	= m_spCmdWidget->getCaption();

		m_spCmdWidget->setCaption("");
		//-----------------------------------
		if (m_pCmdHandler)
			m_pCmdHandler->Execute(sCmd);
		
		time_t t = time(0); 
		KCHAR szTmp[64]; 
		strftime( szTmp, sizeof(szTmp), "%H : %M : %S ->>",localtime(&t) ); 

		PrintLine(szTmp + sCmd);

		//////////////////////////////////////////////////////////////////////////
		m_nCurrIndex = m_kStoredList.size();
		m_kStoredList.push_back(sCmd);
	}
	else if (_key == MyGUI::KeyCode::Tab)
	{
		OnRecept();
	}
	else if (_key == MyGUI::KeyCode::ArrowUp)
	{
		if (m_nCurrIndex >= 0)
		{
			m_spCmdWidget->setCaption(m_kStoredList[m_nCurrIndex]);
			m_nCurrIndex--;
		}
	}
	else if (_key == MyGUI::KeyCode::ArrowDown)
	{
		if (m_nCurrIndex < m_kStoredList.size())
		{
			m_spCmdWidget->setCaption(m_kStoredList[m_nCurrIndex]);
			m_nCurrIndex++;
		}
	}
}

KVOID Kylin::DebugConsole::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::Grave)
	{
		if (IsVisible())
			SetVisible(false);
		else
			SetVisible(true);

		m_spCmdWidget->setCaption("");
	}

}

KVOID Kylin::DebugConsole::SetHandler( CommandHandler* pHandler )
{
	SAFE_DEL(m_pCmdHandler);
	assert(pHandler);
	m_pCmdHandler = pHandler;
}

KVOID Kylin::DebugConsole::PrintLine( KSTR sLine )
{
	int nCount	= m_spContentWidget->getItemCount();

	m_spContentWidget->addItem(sLine, nCount);
	m_spContentWidget->beginToItemAt(nCount);
}