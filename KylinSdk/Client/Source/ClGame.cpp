#include "cltpch.h"
#include "ClGame.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "uiOptionMenu.h"
#include "CameraControl.h"
#include "CCSCameraControlSystem.h"


Kylin::ClGame::ClGame()
{

}

KBOOL Kylin::ClGame::Initialize()
{
	//////////////////////////////////////////////////////////////////////////
	Ogre::Camera* pCam = OgreRoot::GetSingletonPtr()->CreateCamera("$MainCamera");
	if (pCam)
	{
		pCam->setNearClipDistance(0.2f);
		//OgreRoot::GetSingletonPtr()->CreateViewports(pCam);
		OgreRoot::GetSingletonPtr()->CreateCameraControl(pCam);
	}

	if (!GSGame::Initialize())
		return false;

	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	
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
	Kylin::OptionMenu* pOption = KNEW Kylin::OptionMenu();
	pOption->Initialize();	

	//////////////////////////////////////////////////////////////////////////
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pOption);
}
