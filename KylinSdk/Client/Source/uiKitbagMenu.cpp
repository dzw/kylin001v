#include "cltpch.h"
#include "uiKitbagMenu.h"
#include "ItemCell.h"
#include "uiTipsMenu.h"


Kylin::KitbagMenu::KitbagMenu()
: GuiBase(CLASS_TO(KitbagMenu))
, m_pKitbagListener(NULL)
{
	initialiseByAttributes(this);

	m_pItemTips = KNEW TipsMenu();
	m_pItemTips->Hide();
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

	m_pItem_1_1->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_2->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_3->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_4->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_5->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_6->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_1_7->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	//---------------------------------------------------------------
	m_pItem_2_1->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_2->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_3->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_4->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_5->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_6->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_2_7->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);

	m_pItem_2_1->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_2->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_3->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_4->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_5->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_6->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_2_7->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	//---------------------------------------------------------------
	m_pItem_3_1->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_2->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_3->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_4->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_5->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_6->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);
	m_pItem_3_7->eventMouseButtonPressed = newDelegate(this, &KitbagMenu::NotifyClick_Item);

	m_pItem_3_1->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_2->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_3->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_4->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_5->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_6->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	m_pItem_3_7->eventToolTip	 = newDelegate(this, &KitbagMenu::NotifyToolTip);
	//---------------------------------------------------------------

	SetVisible(false);

	return true;
}

KVOID Kylin::KitbagMenu::Destroy()
{
	SAFE_DEL(m_pKitbagListener);
	SAFE_DEL(m_pItemTips);
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
		{
			SetVisible(false);

			m_pItemTips->Hide();
		}
		else
			SetVisible(true);
	}
}

KVOID Kylin::KitbagMenu::SetItemInfo( KINT nIndex,ItemCell* pCell )
{
	static MyGUI::ResourceImageSetPtr resource_item = nullptr;
	resource_item = MyGUI::ResourceManager::getInstance().getByName(pCell->m_sIcon)->castType<MyGUI::ResourceImageSet>();
	MyGUI::StaticImage* pItem = nullptr;

	switch(nIndex)
	{
	case 0:
		
		pItem = m_pItem_1_1;
		break;
	case 1:

		pItem = m_pItem_1_2;
		break;
	case 2:

		pItem = m_pItem_1_3;
		break;
	case 3:

		pItem = m_pItem_1_4;
		break;
	case 4:

		pItem = m_pItem_1_5;
		break;
	case 5:

		pItem = m_pItem_1_6;
		break;
	case 6:

		pItem = m_pItem_1_7;
		break;
	case 7:

		pItem = m_pItem_2_1;
		break;
	case 8:

		pItem = m_pItem_2_2;
		break;
	case 9:

		pItem = m_pItem_2_3;
		break;
	case 10:

		pItem = m_pItem_2_4;
		break;
	case 11:

		pItem = m_pItem_2_5;
		break;
	case 12:

		pItem = m_pItem_2_6;
		break;
	case 13:

		pItem = m_pItem_3_7;
		break;
	case 14:

		pItem = m_pItem_3_1;
		break;
	case 15:

		pItem = m_pItem_3_2;
		break;
	case 16:

		pItem = m_pItem_3_3;
		break;
	case 17:

		pItem = m_pItem_3_4;
		break;
	case 18:

		pItem = m_pItem_3_5;
		break;
	case 19:

		pItem = m_pItem_3_6;
		break;
	case 20:

		pItem = m_pItem_3_7;
		break;
	default:
		assert(NULL);
	}
	
	if (pItem)
	{
		SAFE_CALL(pItem,setItemResourcePtr(resource_item))
		SAFE_CALL(pItem,setItemGroup("States"));
		SAFE_CALL(pItem,setVisible(true));
	}
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
	else if ( _sender == m_pItem_2_1)
		nIndex = 7;
	else if ( _sender == m_pItem_2_2)
		nIndex = 8;
	else if ( _sender == m_pItem_2_3)
		nIndex = 9;
	else if ( _sender == m_pItem_2_4)
		nIndex = 10;
	else if ( _sender == m_pItem_2_5)
		nIndex = 11;
	else if ( _sender == m_pItem_2_6)
		nIndex = 12;
	else if ( _sender == m_pItem_2_7)
		nIndex = 13;
	else if ( _sender == m_pItem_3_1)
		nIndex = 14;
	else if ( _sender == m_pItem_3_2)
		nIndex = 15;
	else if ( _sender == m_pItem_3_3)
		nIndex = 16;
	else if ( _sender == m_pItem_3_4)
		nIndex = 17;
	else if ( _sender == m_pItem_3_5)
		nIndex = 18;
	else if ( _sender == m_pItem_3_6)
		nIndex = 19;
	else if ( _sender == m_pItem_3_7)
		nIndex = 20;

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

	m_pItem_2_1->setVisible(false);
	m_pItem_2_2->setVisible(false);
	m_pItem_2_3->setVisible(false);
	m_pItem_2_4->setVisible(false);
	m_pItem_2_5->setVisible(false);
	m_pItem_2_6->setVisible(false);
	m_pItem_2_7->setVisible(false);

	m_pItem_3_1->setVisible(false);
	m_pItem_3_2->setVisible(false);
	m_pItem_3_3->setVisible(false);
	m_pItem_3_4->setVisible(false);
	m_pItem_3_5->setVisible(false);
	m_pItem_3_6->setVisible(false);
	m_pItem_3_7->setVisible(false);
}

KVOID Kylin::KitbagMenu::NotifyToolTip( MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info )
{
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
	else if ( _sender == m_pItem_2_1)
		nIndex = 7;
	else if ( _sender == m_pItem_2_2)
		nIndex = 8;
	else if ( _sender == m_pItem_2_3)
		nIndex = 9;
	else if ( _sender == m_pItem_2_4)
		nIndex = 10;
	else if ( _sender == m_pItem_2_5)
		nIndex = 11;
	else if ( _sender == m_pItem_2_6)
		nIndex = 12;
	else if ( _sender == m_pItem_2_7)
		nIndex = 13;
	else if ( _sender == m_pItem_3_1)
		nIndex = 14;
	else if ( _sender == m_pItem_3_2)
		nIndex = 15;
	else if ( _sender == m_pItem_3_3)
		nIndex = 16;
	else if ( _sender == m_pItem_3_4)
		nIndex = 17;
	else if ( _sender == m_pItem_3_5)
		nIndex = 18;
	else if ( _sender == m_pItem_3_6)
		nIndex = 19;
	else if ( _sender == m_pItem_3_7)
		nIndex = 20;

	if (nIndex != -1)
	{ 
		KVEC<ItemCell*> kItems = m_pKitbagListener->GetKitbag()->GetItems();
		
		assert(nIndex < (KINT)kItems.size());
		if ( nIndex < (KINT)kItems.size() && 
			 kItems[nIndex] && 
			 !kItems[nIndex]->m_sExplain.empty() )
		{
			m_pItemTips->SetTitle("道具");
			m_pItemTips->SetContent(kItems[nIndex]->m_sExplain);
			m_pItemTips->Show(_info.point);
		}
	}
}