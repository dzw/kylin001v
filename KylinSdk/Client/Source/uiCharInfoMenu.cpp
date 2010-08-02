#include "cltpch.h"
#include "uiCharInfoMenu.h"

#include <MyGUI.h>
#include "AssertX.h"
#include "RenderBoxScene.h"
#include "StringUtils.h"
#include "Entity.h"
#include "KylinRoot.h"


Kylin::CharInfoMenu::CharInfoMenu()
: GuiBase(CLASS_TO(CharInfoMenu))
, m_uTargetID(0xffffffff)
{
	initialiseByAttributes(this);

//	m_pRenderBox = KNEW wraps::RenderBoxScene();
}

KBOOL Kylin::CharInfoMenu::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
// 	const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();
// 
// 	m_pMainWindow = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(size.width - 300, 100, 280, 350), MyGUI::Align::Default, "Main");
// 	m_pMainWindow->setCaption(StringUtils::s2ws("╫ги╚пео╒"));
// 	m_pMainWindow->setMinSize(280, 350);
// 	m_pMainWindow->setNeedKeyFocus(false);
// 
// 	MyGUI::Canvas* canvas2 = m_pMainWindow->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, m_pMainWindow->getClientCoord().width, m_pMainWindow->getClientCoord().height), MyGUI::Align::Stretch);
 	
	m_pMainWindow->setNeedKeyFocus(false);
	
// 	m_pCanvas->setPointer("hand");
// 	m_pRenderBox->setCanvas(m_pCanvas);
// 	m_pRenderBox->setMouseRotation(true);
	
	m_pMainWindow->eventWindowButtonPressed = MyGUI::newDelegate(this, &CharInfoMenu::notifyWindowPressed);

	//-------------------------------------------
	SetVisible(false);

	return true;
}

KVOID Kylin::CharInfoMenu::Destroy()
{
// 	m_pRenderBox->destroy();
// 	SAFE_DEL(m_pRenderBox);
}

KVOID Kylin::CharInfoMenu::SetVisible( KBOOL bVisible )
{
	m_pMainWindow->setVisible(bVisible);

	GuiBase::SetVisible(bVisible);

	if (bVisible)
	{
		Refresh();
	}
}

//KVOID Kylin::CharInfoMenu::SetRenderMyself( Ogre::Entity* pEnt )
//{
// 	Assert(pEnt);
// 	m_pMyself = pEnt;
// 
// 	if (m_pMyself)
// 		m_pRenderBox->injectObject(m_pMyself);
//}

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

KVOID Kylin::CharInfoMenu::Refresh()
{
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_uTargetID);
	if (pEnt)
	{
		PropertySet kProp = pEnt->GetPropertyRef();

		KSTR sName = pEnt->GetName();
			
		KINT nHP = 0;
		kProp.GetIntValue("$HP",nHP);

		KINT nDef = 0;
		kProp.GetIntValue("$DEF",nDef);

		KINT nStr = 0;
		kProp.GetIntValue("$STR",nStr);

		KINT nAtk = 0;
		kProp.GetIntValue("$ATK",nAtk);

		KINT nLevel = 0;
		kProp.GetIntValue("$Level",nLevel);

		m_pTextName->setCaption(StringUtils::s2ws(sName));
		m_pTextHp->setCaption(StringUtils::s2ws(Ogre::StringConverter::toString(nHP)));
		m_pTextDef->setCaption(StringUtils::s2ws(Ogre::StringConverter::toString(nDef)));
		m_pTextStr->setCaption(StringUtils::s2ws(Ogre::StringConverter::toString(nStr)));
		m_pTextAtk->setCaption(StringUtils::s2ws(Ogre::StringConverter::toString(nAtk)));
		m_pTextLevel->setCaption(StringUtils::s2ws(Ogre::StringConverter::toString(nLevel)));
	}
}

KVOID Kylin::CharInfoMenu::notifyWindowPressed( MyGUI::WidgetPtr _widget, const std::string& _name )
{
	MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(_widget);
	if (_name == "close") 
	{
		SetVisible(false);
	}
}