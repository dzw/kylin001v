#include "engpch.h"
#include "GuiManager.h"
#include "CursorEx.h"
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
	m_pPlatform = new MyGUI::OgrePlatform();
	m_pPlatform->initialise(pWindew, pSceneMnger);

	m_pGUI = new MyGUI::Gui();
	m_pGUI->initialise(m_sResource);
	
	//////////////////////////////////////////////////////////////////////////
	// test code update by lby [4/26/2010]
	InitShell(NULL);

	return true;
}

KVOID Kylin::GuiManager::Destroy()
{
	for (GuiMap::iterator it = m_kGuiMap.begin(); it != m_kGuiMap.end(); it++)
	{
		it->second->Destroy();
		KDEL it->second;
	}
	m_kGuiMap.clear();
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
	if (pShell)
		pShell->UiLoader();
	
	//////////////////////////////////////////////////////////////////////////
	Kylin::DebugFrameStats* pFrameStats = KNEW Kylin::DebugFrameStats();
	pFrameStats->Initialize();	

	Kylin::DebugConsole* pConsole = KNEW Kylin::DebugConsole();
	pConsole->Initialize();
	pConsole->SetHandler(KNEW CommandHandler());

	Kylin::CursorEx* pCursor = KNEW Kylin::CursorEx();
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