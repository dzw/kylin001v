#include "cltpch.h"
#include "uiMonsterInfoMenu.h"



Kylin::MonsterInfoMenu::MonsterInfoMenu()
: GuiBase(CLASS_TO(MonsterInfoMenu))
{
	initialiseByAttributes(this);

}

KBOOL Kylin::MonsterInfoMenu::Initialize()
{

	return true;
}

KVOID Kylin::MonsterInfoMenu::Destroy()
{

}

KVOID Kylin::MonsterInfoMenu::SetVisible( KBOOL bVisible )
{

}

KVOID Kylin::MonsterInfoMenu::SetWidgetWidthPct( KSTR sName, KFLOAT fW )
{
// 	if (sName == "image_experience")
// 	{
// 		m_pImageExprience->setSize(int(fW*m_pImageExprience->getWidth()),m_pImageExprience->getHeight());
// 	}
}