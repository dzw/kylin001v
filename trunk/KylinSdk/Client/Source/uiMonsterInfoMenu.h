#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(ShortcutMenu, "monster.layout");
	class MonsterInfoMenu : public GuiBase
						  , public wraps::BaseLayout
	{
	public:
		MonsterInfoMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);

		KVOID SetWidgetWidthPct(KSTR sName, KFLOAT fW);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(MonsterInfoMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;

	};
}