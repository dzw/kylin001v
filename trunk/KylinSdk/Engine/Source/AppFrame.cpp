#include "engpch.h"
#include "AppFrame.h"
#include "rOgreRoot.h"
#include "GuiManager.h"
#include "DataManager.h"
#include "InputManager.h"
#include "CameraControl.h"
#include "ScriptVM.h"
#include "CollisionWrapper.h"
#include "EffectManager.h"


namespace Kylin
{
	AppFrame* g_theApp = NULL;

	AppFrame::AppFrame()
		: m_pRoot(NULL)
		, m_pWindow(NULL)
		, m_pGuiMgr(NULL)
		, m_pSceneMgr(NULL)
		, m_pInputMgr(NULL)
		, m_pScriptVM(NULL)
		, m_bShutDown(false)
		, m_bPaused(false)
	{
		
	}

	AppFrame::~AppFrame()
	{
		//Remove ourself as a Window listener
		Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);
		windowClosed(m_pWindow);

		SAFE_DEL(m_pRoot);
	}

	KBOOL AppFrame::Initialize( KCCHAR* pTitle, KCCHAR* pIcon )
	{
		if (g_theApp) return false;
		else  g_theApp = this;

#ifdef _DEBUG
		m_pRoot = new Ogre::Root("plugins_debug.cfg");
#else
		m_pRoot = new Ogre::Root();
#endif
		//////////////////////////////////////////////////////////////////////////
		if(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog())
		{
			m_pWindow = m_pRoot->initialise(true, pTitle);
			
			if (!LoadResources())
				return false;

			// Set default mipmap level (NB some APIs ignore this)
			Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
			
			Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);
			m_pRoot->addFrameListener(this);

			//////////////////////////////////////////////////////////////////////////
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
			
			CreateWidgets();

			return true;
		}
		
		return false;
	}

	KVOID AppFrame::Run()
	{
		if (m_pRoot)
		{
			m_pRoot->startRendering();
		}

		Destroy();
	}

	KVOID AppFrame::Destroy()
	{
		if (EffectManager::Initialized())
			KDEL EffectManager::GetSingletonPtr();
		if (CollisionWrapper::Initialized())
			KDEL CollisionWrapper::GetSingletonPtr();

		SAFE_DEL(m_pScriptVM);
		SAFE_DEL(m_pGuiMgr);

		SAFE_DEL(m_pCameraCtrl);
		SAFE_DEL(m_pInputMgr);
		
		if (OgreRoot::Initialized())
			KDEL OgreRoot::GetSingletonPtr();

		if (DataManager::Initialized())
			KDEL DataManager::GetSingletonPtr();
	}

	KBOOL AppFrame::LoadResources()
	{
		// Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load("resources.cfg");

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
			}
		}

		return true;
	}

	KVOID AppFrame::CreateWidgets()
	{
 		if (!OgreRoot::Initialized())
 			KNEW OgreRoot();
		if (!DataManager::Initialized())
			KNEW DataManager();
		//////////////////////////////////////////////////////////////////////////
		m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);

		// set shadow properties
		m_pSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		m_pSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
		m_pSceneMgr->setShadowTextureSize(1024);
		m_pSceneMgr->setShadowTextureCount(1);

		Ogre::Camera* pCam = OgreRoot::GetSingletonPtr()->CreateCamera("$MainCamera");
		if (pCam)
		{
			pCam->setNearClipDistance(0.2f);
			
			OgreRoot::GetSingletonPtr()->CreateViewports(pCam);
			//-------------------------------------------------
			m_pCameraCtrl = KNEW CameraControl();
			m_pCameraCtrl->Initialize(m_pSceneMgr,pCam);
		}
		//////////////////////////////////////////////////////////////////////////
		m_pInputMgr = KNEW InputManager();
		m_pInputMgr->Initialize();
		//////////////////////////////////////////////////////////////////////////
		m_pGuiMgr = KNEW GuiManager();
		m_pGuiMgr->Create(m_pWindow,m_pSceneMgr);

		//////////////////////////////////////////////////////////////////////////
		m_pScriptVM = KNEW ScriptVM();
		if(m_pScriptVM->Init())
		{
			OpenScriptBinding(m_pScriptVM->GetLuaState());
			m_pScriptVM->ExecuteScriptFile("./Data/script/startvm.lua");
		}
		//////////////////////////////////////////////////////////////////////////
		if (!CollisionWrapper::Initialized())
			KNEW CollisionWrapper();
		CollisionWrapper::GetSingletonPtr()->Initialize(m_pSceneMgr);
		//////////////////////////////////////////////////////////////////////////
		if (!EffectManager::Initialized())
			KNEW EffectManager();
		EffectManager::GetSingletonPtr()->Initialize(m_pRoot,m_pSceneMgr);
		//////////////////////////////////////////////////////////////////////////
	}

	KVOID AppFrame::OnExit()
	{
		Destroy();
	}
	
	KBOOL AppFrame::frameRenderingQueued( const Ogre::FrameEvent& evt )
	{
		if(m_pWindow->isClosed())
			return false;
		
		if (m_bShutDown)
			return false;
		
		m_pInputMgr->Tick(evt.timeSinceLastFrame);

		if (!m_bPaused)
			OnIdle(evt.timeSinceLastFrame);
	
		return true;
	}

	void AppFrame::windowResized( Ogre::RenderWindow* rw )
	{
		KUINT width, height, depth;
		KINT left, top;
		rw->getMetrics(width, height, depth, left, top);

		OnSize(width,height);
	}

	void AppFrame::windowClosed( Ogre::RenderWindow* rw )
	{
		OnExit();
	}

	KVOID AppFrame::OnIdle( KFLOAT fElapsed )
	{
		m_pCameraCtrl->Update(fElapsed);
		m_pGuiMgr->Update(fElapsed);
		CollisionWrapper::GetSingletonPtr()->Update(fElapsed);
	}

	KVOID AppFrame::Pause(KVOID)
	{
		m_bPaused = true;
	}

	KVOID AppFrame::Resume(KVOID)
	{
		m_bPaused = false;
	}
}