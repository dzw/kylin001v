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
	
	SetVisible(false);

	return true;
}

KVOID Kylin::TaskTipsMenu::Destroy()
{

}

KVOID Kylin::TaskTipsMenu::SetVisible( KBOOL bVisible )
{
	mMainWidget->setVisible(bVisible);
}

KVOID Kylin::TaskTipsMenu::SetContent( KCSTR& sContent )
{
	m_pTextContent->setCaption(StringUtils::s2ws(sContent));
}

KVOID Kylin::TaskTipsMenu::SetResult( KCSTR& sresult )
{
	m_pTextContent->setCaption(StringUtils::s2ws(sresult));
}