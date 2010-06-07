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
		
		KVOID SetRenderMyself(Ogre::Entity* pEnt);
	
	protected:
		virtual KVOID OnKeyDown(KUINT uKey);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pMainWindow, "_Main");
		MyGUI::Window*			m_pMainWindow;
		ATTRIBUTE_FIELD_WIDGET_NAME(CharInfoMenu, m_pCanvas, "render_box");
		MyGUI::Canvas*			m_pCanvas;

		wraps::RenderBoxScene*	m_pRenderBox;
		
		//Ogre::Entity* m_pTarget;
		Ogre::Entity* m_pMyself;
	};
}