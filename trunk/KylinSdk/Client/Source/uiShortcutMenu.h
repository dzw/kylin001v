#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(ShortcutMenu, "shortcut.layout");
	class ShortcutMenu : public GuiBase
					   , public wraps::BaseLayout
	{
	public:
		ShortcutMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);
		
		KVOID SetWidgetHeightPct(KSTR sName, KFLOAT fH);
		KVOID SetWidgetWidthPct(KSTR sName, KFLOAT fW);

		KVOID SetSkillIcon(KSTR sIcon,KCHAR cP);

	public:
		KVOID NotifyClick_Skill_L(MyGUI::Widget* _sender);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageHealth, "image_health");
		MyGUI::StaticImage* m_pImageHealth;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageExprience, "image_experience");
		MyGUI::StaticImage* m_pImageExprience;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageSkill_L, "image_skill_l");
		MyGUI::StaticImage* m_pImageSkill_L;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageSkill_R, "image_skill_r");
		MyGUI::StaticImage* m_pImageSkill_R;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageSkill_1, "image_skill_1");
		MyGUI::StaticImage* m_pImageSkill_1;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageSkill_2, "image_skill_2");
		MyGUI::StaticImage* m_pImageSkill_2;

	protected:

	};
}