#include "cltpch.h"
#include "uiCharInfoMenu.h"

#include <MyGUI.h>
#include "AssertX.h"
#include "RenderBoxScene.h"
#include "StringUtils.h"


Kylin::CharInfoMenu::CharInfoMenu()
: GuiBase(CLASS_TO(CharInfoMenu))
//, m_pTarget(NULL)
, m_pMyself(NULL)
, m_pMainWindow(NULL)
{
	m_pRenderBox = KNEW wraps::RenderBoxScene();
}

KBOOL Kylin::CharInfoMenu::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
	const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();

	m_pMainWindow = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(size.width - 300, 100, 280, 350), MyGUI::Align::Default, "Main");
	m_pMainWindow->setCaption(StringUtils::s2ws("╫ги╚пео╒"));
	m_pMainWindow->setMinSize(280, 350);
	m_pMainWindow->setNeedKeyFocus(false);

	MyGUI::Canvas* canvas2 = m_pMainWindow->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, m_pMainWindow->getClientCoord().width, m_pMainWindow->getClientCoord().height), MyGUI::Align::Stretch);
	canvas2->setPointer("hand");

	m_pRenderBox->setCanvas(canvas2);
	m_pRenderBox->setMouseRotation(true);
	
	//-------------------------------------------
	SetVisible(false);

	return true;
}

KVOID Kylin::CharInfoMenu::Destroy()
{
	m_pRenderBox->destroy();
	SAFE_DEL(m_pRenderBox);
}

KVOID Kylin::CharInfoMenu::SetVisible( KBOOL bVisible )
{
	m_pMainWindow->setVisible(bVisible);

	GuiBase::SetVisible(bVisible);
}

KVOID Kylin::CharInfoMenu::SetRenderMyself( Ogre::Entity* pEnt )
{
	Assert(pEnt);
	m_pMyself = pEnt;

	if (m_pMyself)
		m_pRenderBox->injectObject(m_pMyself);
}

KVOID Kylin::CharInfoMenu::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::C)
	{
		if (this->IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}