#include "engpch.h"
#include "GuiManager.h"
#include "uiCursorEx.h"
#include "uiFrameStats.h"
#include "uiConsole.h"
#include "CommandHandler.h"
#include <MyGUI_OgrePlatform.h>


#define GUI_RESOURCE "core.xml"

//////////////////////////////////////////////////////////////////////////
Kylin::GuiManager::GuiManager()
: m_pGUI(NULL)
, m_pPlatform(NULL)
, m_sResource(GUI_RESOURCE)
{

}

Kylin::GuiManager::~GuiManager()
{
	Destroy();
}

MyGUI::Gui* Kylin::GuiManager::GetGUI()
{
	return m_pGUI;
}

KBOOL Kylin::GuiManager::Create(Ogre::RenderWindow* pWindew, Ogre::SceneManager* pSceneMnger)
{
	//////////////////////////////////////////////////////////////////////////
	m_pPlatform = KNEW MyGUI::OgrePlatform();
	m_pPlatform->initialise(pWindew, pSceneMnger);

	m_pGUI = KNEW MyGUI::Gui();
	m_pGUI->initialise(m_sResource);
	
	//////////////////////////////////////////////////////////////////////////
	MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_blue_tag.xml");
	MyGUI::Gui::getInstance().load("core_skin.xml");
	
	MyGUI::Gui::getInstance().load("game_imageset.xml");
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KVOID Kylin::GuiManager::Destroy()
{
	RemoveAllGui();
	//////////////////////////////////////////////////////////////////////////
	if (m_pGUI)
	{
		m_pGUI->shutdown();
		SAFE_DEL(m_pGUI);
	}

	if (m_pPlatform)
	{
		m_pPlatform->shutdown();
		SAFE_DEL(m_pPlatform);
	}
}

KVOID Kylin::GuiManager::RegisterGui( GuiBase* pGui )
{
	GuiMap::iterator it = m_kGuiMap.find(pGui->m_sGuiName);
	if (it == m_kGuiMap.end())
	{
		m_kGuiMap.insert(std::pair<KSTR,GuiBase*>(pGui->m_sGuiName,pGui));
	}
	else
	{
		assert(NULL);
	}
}

KVOID Kylin::GuiManager::UnregisterGui( GuiBase* pGui )
{
	GuiMap::iterator it = m_kGuiMap.find(pGui->m_sGuiName);
	if (it != m_kGuiMap.end())
	{
		m_kGuiMap.erase(it);
	}
}

KVOID Kylin::GuiManager::InitShell(GuiShell* pShell)
{
	RemoveAllGui();

	if (pShell)
		pShell->UiLoader();
	
	//////////////////////////////////////////////////////////////////////////
	DebugFrameStats* pFrameStats = KNEW DebugFrameStats();
	pFrameStats->Initialize();	

	DebugConsole* pConsole = KNEW DebugConsole();
	pConsole->Initialize();
	pConsole->SetHandler(KNEW CommandHandler());

	CursorEx* pCursor = KNEW CursorEx();
	pCursor->Initialize();
	
	RegisterGui(pFrameStats);
	RegisterGui(pConsole);
	RegisterGui(pCursor);
}

KVOID Kylin::GuiManager::Update( KFLOAT fElapsed )
{
	for (GuiMap::iterator it = m_kGuiMap.begin(); it != m_kGuiMap.end(); it++)
	{
		if (it->second->m_bVisibleFlag)
			it->second->Render(fElapsed);
	}
	
}

Kylin::GuiBase* Kylin::GuiManager::GetGuiBase( KSTR sName )
{
	GuiMap::iterator it = m_kGuiMap.find(sName);
	if (it != m_kGuiMap.end())
	{
		return it->second;
	}
	return NULL;
}

KVOID Kylin::GuiManager::RemoveAllGui()
{
	for (GuiMap::iterator it = m_kGuiMap.begin(); it != m_kGuiMap.end(); it++)
	{
		//-----------------------------------------------------------
		// 加载界面不可销毁
		if (it->first == "LoadingProgress")
			continue;
		//-----------------------------------------------------------
		it->second->Destroy();
		SAFE_DEL(it->second);

		it = m_kGuiMap.erase(it);
	}
	//m_kGuiMap.clear();
}