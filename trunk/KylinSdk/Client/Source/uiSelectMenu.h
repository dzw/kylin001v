#pragma once

#include "GuiManager.h"
#include "BaseLayout/BaseLayout.h"


namespace Kylin
{
	class ItemMenu;
	class TipMenu;
	class ItemBoxContainer;

	class SelectMenu : public GuiBase
	{
	public:
		SelectMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);

		KVOID NotifyNotifyItem(wraps::BaseLayout * _sender, const MyGUI::IBNotifyItemData & _info);
		KVOID NotifyToolTip(wraps::BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemMenu * _data);

	protected:
		TipMenu * mToolTip;
		ItemBoxContainer * mItemBoxV;
	};
}//container