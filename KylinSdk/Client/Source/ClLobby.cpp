#include "cltpch.h"
#include "ClLobby.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "uiLobbyMenu.h"

#include "Stage.h"


Kylin::ClLobby::ClLobby()
: m_pStage(NULL)
{
	m_eStatus = GS_LOBBY_;

}

KBOOL Kylin::ClLobby::Initialize()
{
	OgreRoot::GetSingletonPtr()->CreateSceneRay();
	//////////////////////////////////////////////////////////////////////////
	m_pStage = KNEW Kylin::Stage();
	if (!m_pStage->Initialize())
		return false;
	//////////////////////////////////////////////////////////////////////////	
	// ����ʼ��UI
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	return true;
}

KVOID Kylin::ClLobby::Tick( KFLOAT fElapsed )
{
	if (m_pStage)
		m_pStage->Tick(fElapsed);
}

KVOID Kylin::ClLobby::Destroy()
{
	SAFE_DEL(m_pStage);
	OgreRoot::GetSingletonPtr()->DestroySceneRay();
	OgreRoot::GetSingletonPtr()->DestroyCameraControl();
	// ע���������������
	OgreRoot::GetSingletonPtr()->GetSceneManager()->clearScene();
}

KVOID Kylin::ClLobby::UiLoader()
{
	Kylin::LobbyMenu* pLobby = KNEW Kylin::LobbyMenu();
	pLobby->Initialize();	

	//////////////////////////////////////////////////////////////////////////
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pLobby);
}

