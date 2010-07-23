#include "cltpch.h"
#include "uiTipsMenu.h"
#include "StringUtils.h"


Kylin::TipsMenu::TipsMenu()
{
	initialiseByAttributes(this);

	MyGUI::ISubWidgetText* text = m_pContent->getSubWidgetText();
	const MyGUI::IntCoord& coord = text ? text->getCoord() : MyGUI::IntCoord();
	mOffsetHeight = mMainWidget->getHeight() - coord.height;
}

KVOID Kylin::TipsMenu::Show( const MyGUI::IntPoint & _point )
{
	const MyGUI::IntPoint offset(10, 10);

	MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition() + offset;
	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();

	const MyGUI::IntSize & size = mMainWidget->getSize();
	const MyGUI::IntSize & view_size = gui->getViewSize();

	if ((point.left + size.width) > view_size.width)
	{
		point.left -= offset.left + offset.left + size.width;
	}
	if ((point.top + size.height) > view_size.height)
	{
		point.top -= offset.top + offset.top + size.height;
	}

	MyGUI::ISubWidgetText* text = m_pContent->getSubWidgetText();
	const MyGUI::IntSize& text_size = text ? text->getTextSize() : MyGUI::IntSize();
	mMainWidget->setSize(mMainWidget->getWidth(), mOffsetHeight + text_size.height);

	mMainWidget->setPosition(point);
	mMainWidget->setVisible(true);
}

KVOID Kylin::TipsMenu::Hide()
{
	mMainWidget->setVisible(false);
}

KVOID Kylin::TipsMenu::SetTitle( KCSTR& sTitle )
{
	m_pTitle->setCaption(StringUtils::s2ws(sTitle));
}

KVOID Kylin::TipsMenu::SetContent( KCSTR& sContent )
{
	m_pContent->setCaption(StringUtils::s2ws(sContent));
}