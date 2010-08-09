#pragma once

#include "GuiManager.h"
#include "BaseLayout/BaseLayout.h"
#include "InputListener.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(TaskTipsMenu, "taskTips.layout");
	class TaskTipsMenu : public GuiBase
					   , public wraps::BaseLayout
					   , public InputListener
	{
	public:
		TaskTipsMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy(){}

		virtual KVOID SetVisible(KBOOL bVisible);
	
		KVOID	SetContent(KCSTR& sContent);
		KVOID	SetResult(KCSTR& sresult);

	protected:
		virtual KVOID OnKeyDown(KUINT uKey);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(TaskTipsMenu, m_pTextTitle, "txt_title");
		MyGUI::StaticText* m_pTextTitle;
		ATTRIBUTE_FIELD_WIDGET_NAME(TaskTipsMenu, m_pTextContent, "txt_content");
		MyGUI::Edit* m_pTextContent;
		ATTRIBUTE_FIELD_WIDGET_NAME(TaskTipsMenu, m_pTextResult, "txt_result");
		MyGUI::StaticText* m_pTextResult;
	};
}