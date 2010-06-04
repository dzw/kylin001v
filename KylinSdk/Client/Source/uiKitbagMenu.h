#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"
#include "InputListener.h"


namespace Kylin
{
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

	protected:
		virtual KVOID OnKeyDown(KUINT uKey);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(KitbagMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;

	};
}