#include "cltpch.h"
#include "ClGame.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "uiShortcutMenu.h"
#include "uiOptionMenu.h"
#include "uiCharInfoMenu.h"
#include "uiKitbagMenu.h"
#include "uiConsole.h"
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
	DebugConsole* pConsole = (DebugConsole*)(OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("DebugConsole"));
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
	Kylin::ShortcutMenu* pShortcut = KNEW Kylin::ShortcutMenu();
	pShortcut->Initialize();

	Kylin::CharInfoMenu* pCharInfo = KNEW CharInfoMenu();
	pCharInfo->Initialize();

	KitbagMenu* pKitbag = KNEW KitbagMenu();
	pKitbag->Initialize();

	Kylin::OptionMenu* pOption = KNEW Kylin::OptionMenu();
	pOption->Initialize();	

	//////////////////////////////////////////////////////////////////////////
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pShortcut);
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pCharInfo);
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pKitbag);
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pOption);

}
