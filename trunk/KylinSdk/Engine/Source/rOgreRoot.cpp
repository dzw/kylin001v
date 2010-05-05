#include "engpch.h"
#include "rOgreRoot.h"
#include "AppFrame.h"
#include "GuiManager.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "uiConsole.h"


extern Kylin::AppFrame* g_theApp;

Kylin::OgreRoot::OgreRoot()
{

}

Kylin::OgreRoot::~OgreRoot()
{

}

HWND Kylin::OgreRoot::GetWindowHandle()
{
	HWND hwnd;
	g_theApp->m_pWindow->getCustomAttribute("WINDOW", (void*)&hwnd);

	return hwnd;
}

Ogre::Camera* Kylin::OgreRoot::CreateCamera( KCCHAR* pName )
{
	return g_theApp->m_pSceneMgr->createCamera(pName);
}

Ogre::Viewport* Kylin::OgreRoot::CreateViewports(Ogre::Camera* pCamera, KColor kValue)
{
	Ogre::Viewport* vp = g_theApp->m_pWindow->addViewport(pCamera);
	vp->setBackgroundColour(kValue);

	// Alter the camera aspect ratio to match the viewport
	pCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	return vp;
}

Kylin::ScriptVM* Kylin::OgreRoot::GetScriptVM()
{
	return g_theApp->m_pScriptVM;
}

Ogre::Camera* Kylin::OgreRoot::GetCamera( KCCHAR* pName )
{
	if (g_theApp->m_pSceneMgr->hasCamera(pName))
		return g_theApp->m_pSceneMgr->getCamera(pName);

	return NULL;
}

KVOID Kylin::OgreRoot::GetWindowSize( KUINT& uWidth,KUINT& uHeight )
{
	KUINT depth;
	KINT  left, top;

	g_theApp->m_pWindow->getMetrics(uWidth, uHeight, depth, left, top);
}

KVOID Kylin::OgreRoot::ScreenShot()
{
	g_theApp->m_pWindow->writeContentsToTimestampedFile("ky_", ".jpg");
}

KVOID Kylin::OgreRoot::Resume()
{
	g_theApp->Resume();
}

KBOOL Kylin::OgreRoot::IsPaused()
{
	return g_theApp->m_bPaused;
}

KVOID Kylin::OgreRoot::Pause()
{
	g_theApp->Pause();
}

KVOID Kylin::OgreRoot::ShutDown()
{
	g_theApp->m_bShutDown = true;
}

KBOOL Kylin::OgreRoot::IsKeyDown( KUINT uKeyCode )
{
	return g_theApp->m_pInputMgr->IsKeyDown(uKeyCode);
}

OIS::MouseState Kylin::OgreRoot::GetMouseState()
{
	return g_theApp->m_pInputMgr->m_pMouse->getMouseState();
}

Ogre::RenderTarget::FrameStats Kylin::OgreRoot::GetSystemStats()
{
	return g_theApp->m_pWindow->getStatistics();
}

Ogre::TerrainGroup* Kylin::OgreRoot::CreateTerrainGroup( KINT nMapSize, KFLOAT fWorldSize )
{
	Ogre::TerrainGroup* pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(g_theApp->m_pSceneMgr, Ogre::Terrain::ALIGN_X_Z, nMapSize, fWorldSize);
	pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	return pTerrainGroup;
}

Ogre::SceneManager* Kylin::OgreRoot::GetSceneManager()
{
	return g_theApp->m_pSceneMgr;
}

KVOID Kylin::OgreRoot::RegOptListener( InputListener* pl )
{
	assert(pl);
	g_theApp->m_pInputMgr->m_kContainer.push_back(pl);
}

KVOID Kylin::OgreRoot::UnregOptListener( InputListener* pl )
{
	assert(pl);

	for (size_t i = 0; i < g_theApp->m_pInputMgr->m_kContainer.size(); i++)
	{
		if (g_theApp->m_pInputMgr->m_kContainer[i] == pl)
		{
			g_theApp->m_pInputMgr->m_kContainer.erase(g_theApp->m_pInputMgr->m_kContainer.begin()+i);
			break;
		}
	}
}

KBOOL Kylin::OgreRoot::GetMouseRay( KPoint2 vOrg, Ogre::Ray &kRay )
{
	if(!g_theApp->m_pCameraCtrl->GetActiveCamera()) 
		return false;
	
	Ogre::Viewport* pVP = g_theApp->m_pCameraCtrl->GetActiveCamera()->getViewport();
	KFLOAT fWidth  = pVP->getActualWidth();
	KFLOAT fHeight = pVP->getActualHeight();
	kRay = g_theApp->m_pCameraCtrl->GetActiveCamera()->getCameraToViewportRay(vOrg.x / fWidth, vOrg.y / fHeight);

	return true;
}

Kylin::CameraControl* Kylin::OgreRoot::GetCameraController()
{
	return g_theApp->m_pCameraCtrl;
}

Kylin::GuiManager* Kylin::OgreRoot::GetGuiManager()
{
	return g_theApp->m_pGuiMgr;
}

KBOOL Kylin::OgreRoot::IsOpenConsole()
{
	DebugConsole* pConsole = (DebugConsole*)(OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("DebugConsole"));
	if (pConsole)
		return pConsole->IsVisible();

	return false;
}

KSTR Kylin::OgreRoot::GetRootMedia()
{
	return "./Media";
}

KVOID Kylin::OgreRoot::LoadResource( KSTR sRes )
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		GetRootMedia() + sRes, "FileSystem", "General");
}

Ogre::RenderWindow* Kylin::OgreRoot::GetMainWindow()
{
	return g_theApp->m_pWindow;
}