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
		
		virtual KVOID SetWidgetHeightPct(KSTR sName, KFLOAT fH);
		virtual KVOID SetWidgetWidthPct(KSTR sName, KFLOAT fW);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageHealth, "image_health");
		MyGUI::StaticImage* m_pImageHealth;
		ATTRIBUTE_FIELD_WIDGET_NAME(ShortcutMenu, m_pImageExprience, "image_experience");
		MyGUI::StaticImage* m_pImageExprience;
	};
}