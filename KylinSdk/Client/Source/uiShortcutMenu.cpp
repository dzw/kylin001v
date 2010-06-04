#include "cltpch.h"
#include "uiShortcutMenu.h"


Kylin::ShortcutMenu::ShortcutMenu()
: GuiBase(CLASS_TO(ShortcutMenu))
{
	initialiseByAttributes(this);
}

KBOOL Kylin::ShortcutMenu::Initialize()
{
	static MyGUI::ResourceImageSetPtr resource_back = nullptr;
	resource_back = MyGUI::ResourceManager::getInstance().getByName("pic_shortcut_bar")->castType<MyGUI::ResourceImageSet>();
	
	static MyGUI::ResourceImageSetPtr resource_health = nullptr;
	resource_health = MyGUI::ResourceManager::getInstance().getByName("pic_health")->castType<MyGUI::ResourceImageSet>();

	static MyGUI::ResourceImageSetPtr resource_exp = nullptr;
	resource_exp = MyGUI::ResourceManager::getInstance().getByName("pic_exp")->castType<MyGUI::ResourceImageSet>();

	m_pImageBack->setItemResourcePtr(resource_back);
	m_pImageBack->setItemGroup("States");

	m_pImageHealth->setItemResourcePtr(resource_health);
	m_pImageHealth->setItemGroup("States");
	
	m_pImageExprience->setItemResourcePtr(resource_exp);
	m_pImageExprience->setItemGroup("States");


	m_pImageHealth->setColour(MyGUI::Colour(0.75,0,0,0.85));
	
	//-----------------------------------------------------
	// test code
	SetWidgetHeightPct("image_health",0.6f);
	
	SetWidgetWidthPct("image_experience",0.6f);

	return true;
}

KVOID Kylin::ShortcutMenu::Destroy()
{

}

KVOID Kylin::ShortcutMenu::SetVisible( KBOOL bVisible )
{

}

KVOID Kylin::ShortcutMenu::SetWidgetHeightPct( KSTR sName, KFLOAT fH )
{	
	if (sName == "image_health")
	{
		int nw = m_pImageHealth->getWidth();
		int nh = m_pImageHealth->getHeight();

	}
}

KVOID Kylin::ShortcutMenu::SetWidgetWidthPct( KSTR sName, KFLOAT fW )
{
	if (sName == "image_experience")
	{
		m_pImageExprience->setSize(int(fW*m_pImageExprience->getWidth()),m_pImageExprience->getHeight());
	}
}