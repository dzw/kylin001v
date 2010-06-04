#pragma once

#include "GuiManager.h"
#include "InputListener.h"

namespace wraps
{
	class RenderBoxScene;
}

namespace Kylin
{
	class CharInfoMenu : public GuiBase
					   , public InputListener
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
		wraps::RenderBoxScene*	m_pRenderBox;
		MyGUI::Window*			m_pMainWindow;

		//Ogre::Entity* m_pTarget;
		Ogre::Entity* m_pMyself;
	};
}