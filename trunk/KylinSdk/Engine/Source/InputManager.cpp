#include "engpch.h"
#include "InputManager.h"
#include "rOgreRoot.h"
#include "InputListener.h"
#include "GuiManager.h"


Kylin::InputManager::InputManager()
: m_pInputManager(NULL)
, m_pKeyboard(NULL)
, m_pMouse(NULL)
{

}

Kylin::InputManager::~InputManager()
{
	Destroy();
}

KBOOL Kylin::InputManager::Initialize()
{
	OIS::ParamList pl;
	std::ostringstream windowHndStr;
	
	windowHndStr << (size_t)OgreRoot::GetSingletonPtr()->GetWindowHandle();
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	m_pInputManager = OIS::InputManager::createInputSystem( pl );
	m_pKeyboard		= static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
	m_pMouse		= static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));

	m_pMouse->setEventCallback(this);
	m_pKeyboard->setEventCallback(this);
	
	KUINT uWidth,uHeight;
	OgreRoot::GetSingletonPtr()->GetWindowSize(uWidth,uHeight);
	SetWindowExtents(uWidth,uHeight);

	return true;
}

KVOID Kylin::InputManager::Destroy()
{
	SAFE_DEL(m_pMouse)
	SAFE_DEL(m_pKeyboard)

	OIS::InputManager::destroyInputSystem(m_pInputManager);
}

KVOID Kylin::InputManager::Tick(KFLOAT fElapsed)
{
	m_pMouse->capture();
	m_pKeyboard->capture();
}

KVOID Kylin::InputManager::SetWindowExtents( KINT nWidth, KINT nHeight )
{
	if (m_pMouse)
	{
		//Set Mouse Region.. if window resizes, we should alter this to reflect as well
		const OIS::MouseState &ms = m_pMouse->getMouseState();
		ms.width  = nWidth;
		ms.height = nHeight;
	}
}

//////////////////////////////////////////////////////////////////////////
KBOOL Kylin::InputManager::keyPressed( const OIS::KeyEvent &arg )
{
	OnKeyDown(arg.key);
	
	//////////////////////////////////////////////////////////////////////////
	// ui
	MyGUI::Gui* gui = OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGUI();
	gui->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), (MyGUI::Char)arg.text);
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KBOOL Kylin::InputManager::keyReleased( const OIS::KeyEvent &arg )
{
	OnKeyUp(arg.key);
	
	//////////////////////////////////////////////////////////////////////////
	// ui
	MyGUI::Gui* gui = OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGUI();
	gui->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KBOOL Kylin::InputManager::mouseMoved( const OIS::MouseEvent &arg )
{
	OnMouseMove(arg.state.X.abs,arg.state.Y.abs);

	//////////////////////////////////////////////////////////////////////////
	// ui
	MyGUI::Gui* gui = OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGUI();
	gui->injectMouseMove(arg.state.X.abs,arg.state.Y.abs, arg.state.Z.abs);
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KBOOL Kylin::InputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	switch (id)
	{
	case OIS::MB_Left:
		OnLButtonDown(arg.state.X.abs,arg.state.Y.abs);

		break;
	case OIS::MB_Right:
		OnRButtonDown(arg.state.X.abs,arg.state.Y.abs);

		break;
	case OIS::MB_Middle:
		
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ui
	MyGUI::Gui* gui = OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGUI();
	gui->injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KBOOL Kylin::InputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	switch (id)
	{
	case OIS::MB_Left:
		OnLButtonUp(arg.state.X.abs,arg.state.Y.abs);

		break;
	case OIS::MB_Right:
		OnRButtonUp(arg.state.X.abs,arg.state.Y.abs);

		break;
	case OIS::MB_Middle:
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ui
	MyGUI::Gui* gui = OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGUI();
	gui->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	//////////////////////////////////////////////////////////////////////////

	return true;
}

KBOOL Kylin::InputManager::IsKeyDown( KUINT uKey )
{
	return m_pKeyboard->isKeyDown((KKeyCode)uKey);
}

KVOID Kylin::InputManager::OnMouseMove(KINT nX, KINT nY)
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnMouseMove(nX, nY);
}

KVOID Kylin::InputManager::OnKeyDown( KUINT uKey )
{
	if (uKey == OIS::KC_SYSRQ)   // take a screenshot
	{
		OgreRoot::GetSingletonPtr()->ScreenShot();
	}
	else if (uKey == OIS::KC_ESCAPE)
	{
		OgreRoot::GetSingletonPtr()->ShutDown();
	}
	else if (uKey == OIS::KC_0)
	{
		if ( OgreRoot::GetSingletonPtr()->IsPaused() )
			OgreRoot::GetSingletonPtr()->Resume();
		else
			OgreRoot::GetSingletonPtr()->Pause();
	}
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnKeyDown(uKey);
}

OIS::Mouse* Kylin::InputManager::GetMouse()
{
	return m_pMouse;
}

KVOID Kylin::InputManager::OnKeyUp( KUINT uKey )
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnKeyUp(uKey);
}

KVOID Kylin::InputManager::OnLButtonDown( KINT nX, KINT nY )
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnLButtonDown(nX, nY);
}

KVOID Kylin::InputManager::OnLButtonUp( KINT nX, KINT nY )
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnLButtonUp(nX, nY);
}

KVOID Kylin::InputManager::OnRButtonDown( KINT nX, KINT nY )
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnRButtonDown(nX, nY);
}

KVOID Kylin::InputManager::OnRButtonUp( KINT nX, KINT nY )
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0 ; i < m_kContainer.size(); i++)
		m_kContainer[i]->OnRButtonUp(nX, nY);
}