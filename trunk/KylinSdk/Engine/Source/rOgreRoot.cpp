#include "engpch.h"
#include "rOgreRoot.h"
#include "AppFrame.h"
#include "GuiManager.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "uiConsole.h"
#include "RenderableManager.h"


extern Kylin::AppFrame* g_theApp;

Kylin::OgreRoot::OgreRoot()
: m_pRaySceneQuery(NULL)
, m_pSphereSceneQuery(NULL)
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
	Ogre::Camera* pCam = NULL;
	if (!g_theApp->m_pSceneMgr->hasCamera(pName))
		pCam = g_theApp->m_pSceneMgr->createCamera(pName);
	else
		pCam = g_theApp->m_pSceneMgr->getCamera(pName);
	
	if (pCam)
	{
		pCam->setNearClipDistance(0.1f);
		pCam->setFarClipDistance(VISIBLE_DISTANCE*1.5f);
		pCam->setFOVy(Ogre::Radian(1.0f));
		pCam->setAutoAspectRatio(true);
		pCam->setQueryFlags(0);
	}

	return pCam;
}

Ogre::Viewport* Kylin::OgreRoot::CreateViewports(Ogre::Camera* pCamera, KColor kValue)
{
	Ogre::Viewport* vp = pCamera->getViewport();
	if (!vp)
	{
		vp = g_theApp->m_pWindow->addViewport(pCamera);
		vp->setBackgroundColour(kValue);

		// Alter the camera aspect ratio to match the viewport
		pCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	}


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
	//if(!g_theApp->m_pCameraCtrl->GetActiveCamera()) 
	//	return false;
	
	GetMouseRay(vOrg,kRay,GetCamera("$MainCamera"));

	return true;
}

KBOOL Kylin::OgreRoot::GetMouseRay( KPoint2 vOrg, Ogre::Ray &kRay, Ogre::Camera* pCamera )
{
	Ogre::Viewport* pVP = GetMainWindow()->getViewport(0);
	KFLOAT fWidth  = pVP->getActualWidth();
	KFLOAT fHeight = pVP->getActualHeight();
	kRay = pCamera->getCameraToViewportRay(vOrg.x / fWidth, vOrg.y / fHeight);

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
	DebugConsole* pConsole = GET_GUI_PTR(DebugConsole);
	if (pConsole)
		return pConsole->IsVisible();

	return false;
}

KVOID Kylin::OgreRoot::LoadResource( KSTR sRes )
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		sRes, "FileSystem", "General");
}

Ogre::RenderWindow* Kylin::OgreRoot::GetMainWindow()
{
	return g_theApp->m_pWindow;
}

Kylin::CameraControl* Kylin::OgreRoot::CreateCameraControl( Ogre::Camera* pCamera )
{
	if (pCamera)
	{
		CreateViewports(pCamera);
		//-------------------------------------------------
		SAFE_DEL(g_theApp->m_pCameraCtrl);

		g_theApp->m_pCameraCtrl = KNEW Kylin::CameraControl();
		g_theApp->m_pCameraCtrl->Initialize(OgreRoot::GetSingletonPtr()->GetSceneManager(),pCamera);
	}

	return g_theApp->m_pCameraCtrl;
}

KVOID Kylin::OgreRoot::DestroyCameraControl()
{
	SAFE_DEL(g_theApp->m_pCameraCtrl);
}

//-----------------------------------------------------
// 创建射线交集
KVOID Kylin::OgreRoot::CreateSceneQuery()
{
	if (!m_pRaySceneQuery)
		m_pRaySceneQuery = OgreRoot::GetSingletonPtr()->GetSceneManager()->createRayQuery(Ogre::Ray());
	
	if (!m_pSphereSceneQuery)
		m_pSphereSceneQuery = OgreRoot::GetSingletonPtr()->GetSceneManager()->createSphereQuery(Ogre::Sphere());
}

KVOID Kylin::OgreRoot::DestroyQuery()
{
	if (m_pRaySceneQuery)
	{
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyQuery(m_pRaySceneQuery);
		m_pRaySceneQuery = NULL;
	}

	if (m_pSphereSceneQuery)
	{
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyQuery(m_pSphereSceneQuery);
		m_pSphereSceneQuery = NULL;
	}
}

Kylin::InputManager* Kylin::OgreRoot::GetInputManager()
{
	return g_theApp->m_pInputMgr;
}

KVOID Kylin::OgreRoot::SetWindowIcon( KCCHAR* pIcon )
{
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd = GetWindowHandle();
	
	HINSTANCE hinstance;
	hinstance = GetModuleHandle(NULL);
	//HICON icon = LoadIcon(hinstance, MAKEINTRESOURCE(uIcon));
	HICON icon = LoadIconA(hinstance, pIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, LPARAM(icon));
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, LPARAM(icon));
}

Kylin::RenderableManager* Kylin::OgreRoot::GetRenderableManager()
{
	return g_theApp->m_pRenderableMgr;
}