#include "cltpch.h"
#include "uiKitbagMenu.h"


Kylin::KitbagMenu::KitbagMenu()
: GuiBase(CLASS_TO(KitbagMenu))
{
	initialiseByAttributes(this);
}

KBOOL Kylin::KitbagMenu::Initialize()
{
	static MyGUI::ResourceImageSetPtr resource_back = nullptr;
	resource_back = MyGUI::ResourceManager::getInstance().getByName("pic_kitbag_back")->castType<MyGUI::ResourceImageSet>();

	m_pImageBack->setItemResourcePtr(resource_back);
	m_pImageBack->setItemGroup("States");

	mMainWidget->setNeedKeyFocus(false);

	//-------------------------------------------
	SetVisible(false);

	return true;
}

KVOID Kylin::KitbagMenu::Destroy()
{

}

KVOID Kylin::KitbagMenu::SetVisible( KBOOL bVisible )
{
	mMainWidget->setVisible(bVisible);

	GuiBase::SetVisible(bVisible);
}

KVOID Kylin::KitbagMenu::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::M)
	{
		if (this->IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}