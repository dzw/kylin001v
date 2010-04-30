#pragma once

#include <MyGUI.h>

namespace Kylin
{
	class GuiBase
	{
	public:
		GuiBase():m_bVisibleFlag(false){}
		virtual ~GuiBase(){}

	public:
		virtual KBOOL Initialize() = 0;
		virtual KVOID Render(KFLOAT fElapsed) = 0;
		virtual KVOID Destroy() = 0;
		
		virtual KSTR  GetName() { return m_sGuiName; }

		virtual KVOID SetVisible(KBOOL bVisible) { m_bVisibleFlag = bVisible; }
		virtual KBOOL IsVisible() { return m_bVisibleFlag; }

	protected:
		friend GuiManager;

		KBOOL	m_bVisibleFlag;
		KSTR	m_sGuiName;
	};
	//////////////////////////////////////////////////////////////////////////
	
	class GuiShell
	{
	public:
		GuiShell();

		virtual KVOID UiLoader() = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	typedef KMAP<KSTR,GuiBase*> GuiMap;

	class GuiManager
	{
	public:
		GuiManager();
		virtual ~GuiManager();
	
	public:
		MyGUI::Gui* GetGUI();
		
		KBOOL Create(Ogre::RenderWindow* pWindew, Ogre::SceneManager* pSceneMnger);
		KVOID Destroy();
		
		KVOID RegisterGui(GuiBase* pGui);
		KVOID UnregisterGui(GuiBase* pGui);
		GuiBase* GetGuiBase(KSTR sName);

	public:
		virtual KVOID InitShell(GuiShell* pShell);
		virtual KVOID Update(KFLOAT fElapsed);
	
	protected:
		KSTR					m_sResource;

		MyGUI::Gui*				m_pGUI;
		MyGUI::OgrePlatform*	m_pPlatform;
		//---------------------------------------
	
		GuiMap					m_kGuiMap;
	};
}