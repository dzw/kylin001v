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
#include "CameraControl.h"
#include "UserCommandHandler.h"


Kylin::ClGame::ClGame()
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
	OgreRoot::GetSingletonPtr()->GetSceneManager()->clearScene();
}

KVOID Kylin::ClGame::UiLoader()
{
	ShortcutMenu* pShortcut = KNEW ShortcutMenu();
	pShortcut->Initialize();

	MonsterInfoMenu* pMonsterInfo = KNEW MonsterInfoMenu();
	pMonsterInfo->Initialize();
	
// 	CharInfoMenu* pCharInfo = KNEW CharInfoMenu();
// 	pCharInfo->Initialize();

	KitbagMenu* pKitbag = KNEW KitbagMenu();
	pKitbag->Initialize();

	TaskTipsMenu* pTaskTip = KNEW TaskTipsMenu();
	pTaskTip->Initialize();

	OptionMenu* pOption = KNEW OptionMenu();
	pOption->Initialize();	

	//////////////////////////////////////////////////////////////////////////
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pShortcut);
	
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pMonsterInfo);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pTaskTip);

//	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pCharInfo);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pKitbag);
	OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pOption);

}
