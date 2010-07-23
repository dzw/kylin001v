#pragma once

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(TipsMenu, "commontips.layout");
	class TipsMenu : public wraps::BaseLayout
	{
	public:
		TipsMenu();
	
		KVOID Show( const MyGUI::IntPoint & _point );
		KVOID Hide();
		
		KVOID SetTitle(KCSTR& sTitle);
		KVOID SetContent(KCSTR& sContent);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(TipsMenu, m_pTitle, "title");
		MyGUI::StaticText* m_pTitle;
		ATTRIBUTE_FIELD_WIDGET_NAME(TipsMenu, m_pContent, "content");
		MyGUI::Edit* m_pContent;


		int mOffsetHeight;
	};
}