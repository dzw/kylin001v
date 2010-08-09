#include "stdafx.h"
#include "GameApp.h"
#include "DotSceneLoader.h"
#include "Utility.h"
#include "../Resource.h"

#define OGRE_MEMORY_TRACKER_DEBUG_MODE 1

//-------------------------------------------------------------------------------------
GameApp::GameApp(void)
{

}

//-------------------------------------------------------------------------------------
GameApp::~GameApp(void)
{
	
}

//-------------------------------------------------------------------------------------
//void GameApp::setupResources(void)
KBOOL GameApp::LoadResources(void)
{
	if (!GameClient::LoadResources())
		return false;

	return true;
}

KVOID GameApp::OnIdle(KFLOAT fElapsed)
{
	GameClient::OnIdle(fElapsed);
	
}

Kylin::SceneLoader* GameApp::CreateSceneLoader()
{
	return static_cast<Kylin::SceneLoader*>(KNEW DotSceneLoader());
}

//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

	KVOID DoWork(KCHAR *argv)
	{
		GameApp app;
#ifdef _DEBUG
		if (app.Initialize("OGRE Sample ...",MAKEINTRESOURCE(IDR_MAINFRAME)))
#else
		if (app.Initialize("∆Ê√≈",MAKEINTRESOURCE(IDR_MAINFRAME)))
#endif // _DEBUG
		{
			app.Entrance(argv);
			app.Run();
		}
	}

	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT nArgc)
	{
		// Create application object
#ifndef _DEBUG
		try
#endif
		{
			DoWork(strCmdLine);
		} 
#ifndef _DEBUG
 		catch(Ogre::Exception &e)
 		{
 			Ogre::LogManager::getSingleton().logMessage(e.getDescription());
 		}
#endif

		return 0;
	}

#ifdef __cplusplus
}
#endif
