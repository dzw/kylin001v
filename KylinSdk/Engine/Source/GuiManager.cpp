#include "engpch.h"
#include "GuiManager.h"
#include "uiCursorEx.h"
#include "uiFrameStats.h"
#include "uiConsole.h"
#include "CommandHandler.h"
#include "Profiling.h"
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
// 	if (!CProfiling::Initialized())
// 		KNEW CProfiling();
// 	CProfiling::GetSingletonPtr()->Init();

	return true;
}

KVOID Kylin::GuiManager::Destroy()
{
	if (CProfiling::Initialized())
		KDEL CProfiling::GetSingletonPtr();

	RemoveAllGui(true);
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
	
// 	if(	CProfiling::Initialized() && CProfiling::GetSingleton().m_bShow==true)
// 	{
// 		CProfiling::GetSingleton().Render( fElapsed );
// 	}
}

Kylin::GuiBase* Kylin::GuiManager::GetGuiBase( KSTR sName )
{
	// ����ʹ�ú������#classname ���ܰ������ֿռ�
	// �˴����˵����ֿռ�
	KUINT n = sName.find("Kylin::");
	if (n == 0)
		sName = sName.substr(7,sName.length()-1);

	GuiMap::iterator it = m_kGuiMap.find(sName);
	if (it != m_kGuiMap.end())
	{
		return it->second;
	}
	return NULL;
}

KVOID Kylin::GuiManager::RemoveAllGui(KBOOL bFlag)
{
	for (GuiMap::iterator it = m_kGuiMap.begin(); it != m_kGuiMap.end();)
	{
		//-----------------------------------------------------------
		// ���ؽ��治������
		if (!bFlag && it->first == "LoadingProgress")
		{
			++it;
			continue;
		}
		//-----------------------------------------------------------
		it->second->Destroy();
		SAFE_DEL(it->second);

		it = m_kGuiMap.erase(it);
	}
	//m_kGuiMap.clear();
}