#include "stdafx.h"
#include "GameApp.h"
#include "DotSceneLoader.h"
#include "Utility.h"

//-------------------------------------------------------------------------------------
GameApp::GameApp(void)
: mTerrainImported(true),
mSceneFile(Ogre::StringUtil::BLANK),
mHelpInfo(Ogre::StringUtil::BLANK),
mFallVelocity(0)
{
	mHelpInfo = Ogre::String("Use [W][A][S][D] keys for movement.\nKeys [1]-[9] to switch between cameras.\n[0] toggles SceneNode debug visuals.\n\nPress [C] to toggle clamp to terrain (gravity).\n\n[G] toggles the detail panel.\n[R] cycles polygonModes (Solid/Wireframe/Points).\n[T] cycles various filtering.\n\n\nPress [ESC] to quit.");
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
		if (app.Initialize("OGRE Sample ...","kylin.ico"))
		{
			app.Entrance(argv);
			app.Run();
		}
	}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT nArgc)
#else
	int main(KINT argc, KCHAR *argv[])
#endif
	{
		// Create application object
//#ifndef _DEBUG
//		__try
//#else
		try
//#endif
		{
			DoWork(strCmdLine);
		} 
//#ifndef _DEBUG
//		__except(GenerateDump(GetExceptionInformation())){}
//#else
		catch(Ogre::Exception &e)
		{
			Ogre::LogManager::getSingleton().logMessage(e.getDescription());
		}
//#endif

		return 0;
	}

#ifdef __cplusplus
}
#endif
