#include "cltpch.h"
#include "uiMonsterInfoMenu.h"

#include "KylinRoot.h"


Kylin::MonsterInfoMenu::MonsterInfoMenu()
: GuiBase(CLASS_TO(MonsterInfoMenu))
{
	initialiseByAttributes(this);

}

KBOOL Kylin::MonsterInfoMenu::Initialize()
{
	MyGUI::ResourceImageSetPtr resource_head_back = nullptr;
	resource_head_back = MyGUI::ResourceManager::getInstance().getByName("pic_monster_head_b")->castType<MyGUI::ResourceImageSet>();

	MyGUI::ResourceImageSetPtr resource_head_front = nullptr;
	resource_head_front = MyGUI::ResourceManager::getInstance().getByName("pic_monster_1")->castType<MyGUI::ResourceImageSet>();

	MyGUI::ResourceImageSetPtr resource_anima_back = nullptr;
	resource_anima_back = MyGUI::ResourceManager::getInstance().getByName("pic_monster_anima_b")->castType<MyGUI::ResourceImageSet>();

	MyGUI::ResourceImageSetPtr resource_anima_front = nullptr;
  	resource_anima_front = MyGUI::ResourceManager::getInstance().getByName("pic_monster_anima_f")->castType<MyGUI::ResourceImageSet>();

	//-----------------------------------------------------
	m_pImageHeadBack->setItemResourcePtr(resource_head_back);
	m_pImageHeadBack->setItemGroup("States");

	m_pImageHeadFront->setItemResourcePtr(resource_head_front);
	m_pImageHeadFront->setItemGroup("States");

	m_pImageAnimaBack->setItemResourcePtr(resource_anima_back);
	m_pImageAnimaBack->setItemGroup("States");

	m_pImageAnimaFront->setItemResourcePtr(resource_anima_front);
	m_pImageAnimaFront->setItemGroup("States");

	//-----------------------------------------------------
	// test code

	SetWidgetWidthPct("image_anima_front",0.6f);

	SetVisible(false);

	return true;
}

KVOID Kylin::MonsterInfoMenu::Destroy()
{

}

KVOID Kylin::MonsterInfoMenu::SetVisible( KBOOL bVisible )
{
	m_pImageHeadBack->setVisible(bVisible);
	m_pImageHeadFront->setVisible(bVisible);
	m_pImageAnimaBack->setVisible(bVisible);
	m_pImageAnimaFront->setVisible(bVisible);
	m_pTextName->setVisible(bVisible);
}

KVOID Kylin::MonsterInfoMenu::SetWidgetWidthPct( KSTR sName, KFLOAT fW )
{
	if (sName == "image_anima_front")
	{
		m_pImageAnimaFront->setSize(int(fW*m_pImageAnimaFront->getWidth()),m_pImageAnimaFront->getHeight());
	}
}

KVOID Kylin::MonsterInfoMenu::SetTitle( KCSTR sTitle )
{
	m_pTextName->setCaption(sTitle.data());
	
}