#pragma once

#include "Engine.h"

namespace Kylin
{
	class InputManager : public OIS::KeyListener
					   , public OIS::MouseListener
	{
	public:
		InputManager();
		virtual ~InputManager();
		
		virtual KBOOL Initialize();
		virtual KVOID Destroy();
		virtual KVOID Tick(KFLOAT fElapsed);

		KVOID SetWindowExtents(KINT nWidth, KINT nHeight);
		KVOID SetMousePosition(int _x, int _y);
		KBOOL IsKeyDown(KUINT uKey);

		OIS::Mouse* GetMouse();
		
	protected:
		virtual KVOID OnKeyDown(KUINT uKey);
		virtual KVOID OnKeyUp(KUINT uKey);
		
		virtual KVOID OnLButtonDown(KINT nX, KINT nY);
		virtual KVOID OnLButtonUp(KINT nX, KINT nY);
		virtual KVOID OnRButtonDown(KINT nX, KINT nY);
		virtual KVOID OnRButtonUp(KINT nX, KINT nY);
		//virtual KVOID OnMButtonDown(KINT nX, KINT nY){}
		//virtual KVOID OnMButtonUp(KINT nX, KINT nY){}
		virtual KVOID OnMouseMove(KINT nX, KINT nY);
		virtual KVOID OnMouseMove(KFLOAT fX, KFLOAT fY, KFLOAT fZ);

	private:
		virtual KBOOL keyPressed( const OIS::KeyEvent &arg );
		virtual KBOOL keyReleased( const OIS::KeyEvent &arg );
		virtual KBOOL mouseMoved( const OIS::MouseEvent &arg );
		virtual KBOOL mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual KBOOL mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	protected:
		friend class OgreRoot;
			
		//OIS Input devices
		OIS::InputManager*	m_pInputManager;
		OIS::Mouse*			m_pMouse;
		OIS::Keyboard*		m_pKeyboard;

		KVEC<InputListener*> m_kContainer;
	};
}