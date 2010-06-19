#pragma once

#include "Engine.h"

//#define KT_FPS_

namespace Kylin
{
	class AppFrame : public Ogre::FrameListener
				   , public Ogre::WindowEventListener
	{
	public:
		AppFrame();
		virtual ~AppFrame();
		
		virtual KBOOL	Initialize(KCCHAR* pTitle, KCCHAR* pIcon);
		virtual KVOID	Run();
		virtual KVOID	Destroy();
		
		virtual KVOID	Pause(KVOID) ;
		virtual KVOID	Resume(KVOID);

	protected:
		virtual KBOOL	LoadResources();
		virtual KVOID	DestroyScene(){}
		virtual KVOID	CreateWidgets();
		virtual KVOID	OpenScriptBinding(lua_State *L){}

	protected:
		virtual KVOID	OnIdle(KFLOAT fElapsed);
		virtual KVOID	OnExit();
		virtual KVOID	OnSize(KUINT nWidth, KUINT nHeight){}	
		virtual KVOID	OnStartTick();

	private:
		virtual KBOOL frameRenderingQueued(const Ogre::FrameEvent& evt);

		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

	protected:
		friend class OgreRoot;

		Ogre::Root*			m_pRoot;
		Ogre::RenderWindow*	m_pWindow;
		Ogre::SceneManager* m_pSceneMgr;
		
		GuiManager*			m_pGuiMgr;
		InputManager*		m_pInputMgr;
		CameraControl*		m_pCameraCtrl;
		ScriptVM*			m_pScriptVM;
	
		KBOOL m_bShutDown;
		KBOOL m_bPaused;
		KBOOL m_bStartTick;
	};
}