#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"
#include "InputListener.h"
#include "Kitbag.h"


namespace Kylin
{
	class TipsMenu;

	ATTRIBUTE_CLASS_LAYOUT(KitbagMenu, "kitbag.layout");
	class KitbagMenu : public GuiBase
					 , public InputListener
					 , public wraps::BaseLayout
	{
	public:
		KitbagMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);
		
		KVOID	SetItemInfo(KINT nIndex,ItemCell* pCell);
		KVOID	RegKitbagListener(Kitbag::KitbagListener* pListener);
		KVOID	Refresh();
		
	public:
		KVOID NotifyClick_Item(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		KVOID NotifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

	protected:
		virtual KVOID OnKeyDown(KUINT uKey);
		
		KVOID	Clear();

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_1, "img_1_1");
		MyGUI::StaticImage* m_pItem_1_1;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_2, "img_1_2");
		MyGUI::StaticImage* m_pItem_1_2;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_3, "img_1_3");
		MyGUI::StaticImage* m_pItem_1_3;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_4, "img_1_4");
		MyGUI::StaticImage* m_pItem_1_4;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_5, "img_1_5");
		MyGUI::StaticImage* m_pItem_1_5;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_6, "img_1_6");
		MyGUI::StaticImage* m_pItem_1_6;
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_1_7, "img_1_7");
		MyGUI::StaticImage* m_pItem_1_7;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_1, "img_2_1");
// 		MyGUI::StaticImage* m_pItem_2_1;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_2, "img_2_2");
// 		MyGUI::StaticImage* m_pItem_2_2;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_3, "img_2_3");
// 		MyGUI::StaticImage* m_pItem_2_3;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_4, "img_2_4");
// 		MyGUI::StaticImage* m_pItem_2_4;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_5, "img_2_5");
// 		MyGUI::StaticImage* m_pItem_2_5;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_6, "img_2_6");
// 		MyGUI::StaticImage* m_pItem_2_6;
// 		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pItem_2_7, "img_2_7");
// 		MyGUI::StaticImage* m_pItem_2_7;
		
	private:
		Kitbag::KitbagListener*		m_pKitbagListener;

		TipsMenu*					m_pItemTips;
	};
}