#include "cltpch.h"
#include "uiTaskTipsMenu.h"
#include "StringUtils.h"


Kylin::TaskTipsMenu::TaskTipsMenu()
: GuiBase(CLASS_TO(TaskTipsMenu))
{
	initialiseByAttributes(this);
}

KBOOL Kylin::TaskTipsMenu::Initialize()
{
	m_pTextContent->setEditMultiLine(true);

	SetVisible(true);

	return true;
}


KVOID Kylin::TaskTipsMenu::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	mMainWidget->setVisible(bVisible);
}

KVOID Kylin::TaskTipsMenu::SetContent( KCSTR& sContent )
{
	//m_pTextContent->setCaption(StringUtils::s2ws(sContent));
	m_pTextContent->setCaption(sContent);
}

KVOID Kylin::TaskTipsMenu::SetResult( KCSTR& sresult )
{
	m_pTextResult->setCaption(StringUtils::s2ws(sresult));
}

KVOID Kylin::TaskTipsMenu::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::Escape)
	{
		if (IsVisible())
			SetVisible(false);
	}
}