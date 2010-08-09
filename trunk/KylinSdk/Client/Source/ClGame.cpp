#include "cltpch.h"
#include "ClGame.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "uiShortcutMenu.h"
#include "uiOptionMenu.h"
#include "uiCharInfoMenu.h"
#include "uiKitbagMenu.h"
#include "uiConsole.h"
#include "uiMonsterInfoMenu.h"
#include "uiTaskTipsMenu.h"
#include "uiMiniMapMenu.h"
#include "uiHelpMenu.h"
#include "CameraControl.h"
#include "UserCommandHandler.h"
#include "ClLobby.h"


Kylin::ClGame::ClGame()
: m_bQuitFlag(false)
{

}

KBOOL Kylin::ClGame::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
	Ogre::Camera* pCam = OgreRoot::GetSingletonPtr()->CreateCamera("$MainCamera");
	if (pCam)
	{
		pCam->setNearClipDistance(1.0f);
		OgreRoot::GetSingletonPtr()->CreateCameraControl(pCam);
	}
	
	//-----------------------------------------------------
	// 初始化UI
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	// 设置控制台命令接口
	DebugConsole* pConsole = GET_GUI_PTR(DebugConsole);
	pConsole->SetHandler(KNEW UserCommandHandler());
	//-----------------------------------------------------
	if (!GSGame::Initialize())
		return false;


	return true;
}

KVOID Kylin::ClGame::Destroy()
{
	GSGame::Destroy();
	
	OgreRoot::GetSingletonPtr()->DestroyCameraControl();
	// 注：摄像机不被销毁
	//OgreRoot::GetSingletonPtr()->GetSceneManager()->clearScene();
}

KVOID Kylin::ClGame::UiLoader()
{
	MiniMapMenu* pMiniMap = KNEW MiniMapMenu();
	pMiniMap->Initialize();

	ShortcutMenu* pShortcut = KNEW ShortcutMenu();
	pShortcut->Initialize();

	MonsterInfoMenu* pMonsterInfo = KNEW MonsterInfoMenu();
	pMonsterInfo->Initialize();
	
 	CharInfoMenu* pCharInfo = KNEW CharInfoMenu();
 	pCharInfo->Initialize();

	KitbagMenu* pKitbag = KNEW KitbagMenu();
	pKitbag->Initialize();

	HelpMenu* pHelp = KNEW HelpMenu();
	pHelp->Initialize();

	OptionMenu* pOption = KNEW OptionMenu();
	pOption->Initialize();	
	
	// 按键冲突处理
	TaskTipsMenu* pTaskTip = KNEW TaskTipsMenu();
	pTaskTip->Initialize();

	//////////////////////////////////////////////////////////////////////////
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pMiniMap);

	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pShortcut);
	
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pMonsterInfo);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pTaskTip);

	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pCharInfo);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pKitbag);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pHelp);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pOption);

}

KVOID Kylin::ClGame::Tick( KFLOAT fElapsed )
{
	GSGame::Tick(fElapsed);

	if (m_bQuitFlag)
	{
		OgreRoot::GetSingletonPtr()->ShutDown();// SwitchStatus(KNEW ClLobby());
		m_bQuitFlag = false;
	}
}