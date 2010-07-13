#include "cltpch.h"
#include "uiTaskTipsMenu.h"


Kylin::TaskTipsMenu::TaskTipsMenu()
: GuiBase(CLASS_TO(TaskTipsMenu))
{

}

KBOOL Kylin::TaskTipsMenu::Initialize()
{
	

	return true;
}

KVOID Kylin::TaskTipsMenu::Destroy()
{

}

KVOID Kylin::TaskTipsMenu::SetVisible( KBOOL bVisible )
{
	this->setVisible(bVisible);
}