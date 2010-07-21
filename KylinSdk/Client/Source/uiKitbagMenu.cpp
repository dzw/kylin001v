#include "cltpch.h"
#include "uiKitbagMenu.h"
#include "ItemCell.h"


Kylin::KitbagMenu::KitbagMenu()
: GuiBase(CLASS_TO(KitbagMenu))
, m_pKitbagListener(NULL)
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

	//---------------------------------------------------------------
	m_pItem_1_1->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_2->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_3->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_4->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_5->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_6->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_1_7->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	//---------------------------------------------------------------
	// test code

	static MyGUI::ResourceImageSetPtr resource_item_1 = nullptr;
	resource_item_1 = MyGUI::ResourceManager::getInstance().getByName("pic_item_big_hp")->castType<MyGUI::ResourceImageSet>();

	m_pItem_1_1->setItemResourcePtr(resource_item_1);
	m_pItem_1_1->setItemGroup("States");

	//-------------------------------------------
	SetVisible(false);

	return true;
}

KVOID Kylin::KitbagMenu::Destroy()
{
	SAFE_DEL(m_pKitbagListener);
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

KVOID Kylin::KitbagMenu::SetItemInfo( KINT nIndex,ItemCell* pCell )
{
	static MyGUI::ResourceImageSetPtr resource_item = nullptr;
	resource_item = MyGUI::ResourceManager::getInstance().getByName(pCell->m_sIcon)->castType<MyGUI::ResourceImageSet>();
	MyGUI::StaticImage* m_pItem = nullptr;

	switch(nIndex)
	{
	case 0:
		
		m_pItem = m_pItem_1_1;
		break;
	case 1:

		m_pItem = m_pItem_1_2;
		break;
	case 2:

		m_pItem = m_pItem_1_3;
		break;
	case 3:

		m_pItem = m_pItem_1_4;
		break;
	case 4:

		m_pItem = m_pItem_1_5;
		break;
	case 5:

		m_pItem = m_pItem_1_6;
		break;
	case 6:

		m_pItem = m_pItem_1_7;
		break;
	default:
		assert(NULL);
	}
	
	SAFE_CALL(m_pItem,setItemResourcePtr(resource_item))
	SAFE_CALL(m_pItem,setItemGroup("States"));
	SAFE_CALL(m_pItem,setVisible(true));
}

KVOID Kylin::KitbagMenu::RegKitbagListener( Kitbag::KitbagListener* pListener )
{
	SAFE_DEL(m_pKitbagListener);
	Assert(pListener);
	m_pKitbagListener = pListener;
}

KVOID Kylin::KitbagMenu::NotifyClick_Item( MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id )
{
	if (_id != MyGUI::MouseButton::Right)
		return;

	KINT nIndex = -1;
	if ( _sender == m_pItem_1_1)
		nIndex = 0;
	else if ( _sender == m_pItem_1_2)
		nIndex = 1;
	else if ( _sender == m_pItem_1_3)
		nIndex = 2;
	else if ( _sender == m_pItem_1_4)
		nIndex = 3;
	else if ( _sender == m_pItem_1_5)
		nIndex = 4;
	else if ( _sender == m_pItem_1_6)
		nIndex = 5;
	else if ( _sender == m_pItem_1_7)
		nIndex = 6;

	if (nIndex != -1)
	{ 
		SAFE_CALL(m_pKitbagListener,OnUsed(nIndex)); 
		Refresh();
	}
}

KVOID Kylin::KitbagMenu::Refresh()
{
	AssertEx(m_pKitbagListener,"背包监听不可为空！");
	if (!m_pKitbagListener)
		return;
		
	Clear();

	KVEC<ItemCell*> kItems = m_pKitbagListener->GetKitbag()->GetItems();
	
	for (KUINT i = 0; i < kItems.size(); i++)
	{
		SetItemInfo(i,kItems[i]);
	}

}

KVOID Kylin::KitbagMenu::Clear()
{
	m_pItem_1_1->setVisible(false);
	m_pItem_1_2->setVisible(false);
	m_pItem_1_3->setVisible(false);
	m_pItem_1_4->setVisible(false);
	m_pItem_1_5->setVisible(false);
	m_pItem_1_6->setVisible(false);
	m_pItem_1_7->setVisible(false);
}