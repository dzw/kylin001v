#include "cltpch.h"
#include "uiTaskTipsMenu.h"


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