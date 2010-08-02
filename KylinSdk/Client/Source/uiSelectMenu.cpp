#include "cltpch.h"
#include "uiSelectMenu.h"
#include "uiItemBoxContainer.h"
#include "uiItemMenu.h"
#include "uiTipMenu.h"
#include "ClLobby.h"
#include "LobbyScene.h"
#include "KylinRoot.h"


Kylin::SelectMenu::SelectMenu()
: GuiBase(CLASS_TO(SelectMenu))
, mToolTip(NULL)
, mItemBoxV(NULL)
{

}

KBOOL Kylin::SelectMenu::Initialize()
{
	MyGUI::FactoryManager::getInstance().registerFactory<ResourceItemInfo>("Resource");

	MyGUI::Gui::getInstance().load("lobby_imageset.xml");
	MyGUI::Gui::getInstance().load("ItemBox_skin.xml");
	
	mToolTip = KNEW TipMenu();
	mToolTip->Hide();

	mItemBoxV = KNEW ItemBoxContainer("ItemBoxV.layout");
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero0", 3));
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero1", 3));
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero2", 3));
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero3", 3));
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero4", 3));
	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero5", 3));
// 	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero6", 3));
// 	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero7", 3));
// 	mItemBoxV->getItemBox()->addItem(KNEW ItemMenu("info_Hero8", 3));


	mItemBoxV->getItemBox()->eventNotifyItem = newDelegate(this, &SelectMenu::NotifyNotifyItem);
	mItemBoxV->getItemBox()->eventToolTip	 = newDelegate(this, &SelectMenu::NotifyToolTip);


	return true;
}

KVOID Kylin::SelectMenu::Render( KFLOAT fElapsed )
{
	
}

KVOID Kylin::SelectMenu::Destroy()
{
	MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceItemInfo>("Resource");
	//---------------------------------------------------------------
	SAFE_DEL(mItemBoxV);
	SAFE_DEL(mToolTip);

}

KVOID Kylin::SelectMenu::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);
}

KVOID Kylin::SelectMenu::NotifyToolTip( wraps::BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemMenu * _data )
{
	if (_info.type == MyGUI::ToolTipInfo::Show)
	{
		mToolTip->Show(_data, _info.point);
	}
	else if (_info.type == MyGUI::ToolTipInfo::Hide)
	{
		mToolTip->Hide();
	}
}

KVOID Kylin::SelectMenu::NotifyNotifyItem(wraps::BaseLayout * _sender, const MyGUI::IBNotifyItemData & _info)
{
// 	if (_info.index != MyGUI::ITEM_NONE)
// 	{
// 		if (_info.notify == MyGUI::IBNotifyItemData::MouseReleased)
// 		{
// 			Kylin::ClLobby* pLobby = static_cast<ClLobby*>(KylinRoot::GetSingletonPtr()->GetCurrentGameStatus());
// 			if (pLobby)
// 			{
// 				pLobby->GetLobbyScene()->SpawnActor(1);
// 			}
// 		}
// 	}
}
