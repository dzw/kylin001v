#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(MonsterInfoMenu, "monster.layout");
	class MonsterInfoMenu : public GuiBase
						  , public wraps::BaseLayout
	{
	public:
		MonsterInfoMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);
		

		KVOID SetTitle(KCSTR sTitle);
		KVOID SetHPWidthPct(KFLOAT fW);
		
	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pImageHeadBack, "_Main");
		MyGUI::StaticImage* m_pImageHeadBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pImageHeadFront, "image_head");
		MyGUI::StaticImage* m_pImageHeadFront;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pTextName, "txt_title");
		MyGUI::StaticText* m_pTextName;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pImageAnimaBack, "image_anima_back");
		MyGUI::StaticImage* m_pImageAnimaBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pImageAnimaFront, "image_anima_front");
		MyGUI::StaticImage* m_pImageAnimaFront;

	};
}