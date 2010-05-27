#include "cltpch.h"
#include "ClLobby.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"

#include "uiLobbyMenu.h"
#include "uiSelectMenu.h"

#include "LobbyScene.h"


Kylin::ClLobby::ClLobby()
: m_pLobbyScene(NULL)
{
	m_eStatus = GS_LOBBY_;

}

KBOOL Kylin::ClLobby::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
	m_pLobbyScene = KNEW Kylin::LobbyScene();
	if (!m_pLobbyScene->Initialize())
		return false;
	//////////////////////////////////////////////////////////////////////////	
	// 最后初始化UI
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	return true;
}

KVOID Kylin::ClLobby::Tick( KFLOAT fElapsed )
{
	if (m_pLobbyScene)
		m_pLobbyScene->Tick(fElapsed);
}

KVOID Kylin::ClLobby::Destroy()
{
	SAFE_DEL(m_pLobbyScene);
	OgreRoot::GetSingletonPtr()->DestroyCameraControl();
	// 注：摄像机不被销毁
	OgreRoot::GetSingletonPtr()->GetSceneManager()->clearScene();
}

KVOID Kylin::ClLobby::UiLoader()
{
	LobbyMenu* pLobby = KNEW LobbyMenu();
	pLobby->Initialize();	
	
	SelectMenu* pSelect = KNEW SelectMenu();
	pSelect->Initialize();

	//////////////////////////////////////////////////////////////////////////
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pLobby);
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pSelect);
}

Kylin::LobbyScene* Kylin::ClLobby::GetLobbyScene()
{
	return m_pLobbyScene;
}
