#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"
#include "InputListener.h"

namespace wraps
{
	class RenderBoxScene;
}

namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(CharInfoMenu, "CharInfo.layout");
	class CharInfoMenu : public GuiBase
					   , public InputListener
					   , public wraps::BaseLayout
	{
	public:
		CharInfoMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);
		
		//KVOID SetRenderMyself(Ogre::Entity* pEnt);
		KVOID	SetTarget(KUINT uTarget) { m_uTargetID = uTarget; }
		KVOID	Refresh();

	protected:
		virtual KVOID notifyWindowPressed(MyGUI::WidgetPtr _widget, const std::string& _name);

		virtual KVOID OnKeyDown(KUINT uKey);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pMainWindow, "_Main");
		MyGUI::Window*			m_pMainWindow;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextName, "txt_name_v");
		MyGUI::StaticText* m_pTextName;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextLevel, "txt_level_v");
		MyGUI::StaticText* m_pTextLevel;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextHp, "txt_hp_v");
		MyGUI::StaticText* m_pTextHp;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextAtk, "txt_atk_v");
		MyGUI::StaticText* m_pTextAtk;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextDef, "txt_def_v");
		MyGUI::StaticText* m_pTextDef;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pTextStr, "txt_str_v");
		MyGUI::StaticText* m_pTextStr;


// 		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pCanvas, "render_box");
// 		MyGUI::Canvas*			m_pCanvas;

//		wraps::RenderBoxScene*	m_pRenderBox;
		
		//Ogre::Entity* m_pTarget;
//		Ogre::Entity* m_pMyself;

		KUINT	m_uTargetID;
	};
}