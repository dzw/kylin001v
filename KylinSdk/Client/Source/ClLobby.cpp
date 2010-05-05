#include "cltpch.h"
#include "ClLobby.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "uiLobbyMenu.h"
#include "CameraControl.h"
#include "CCSCameraControlSystem.h"


Kylin::ClLobby::ClLobby()
: m_pNode(NULL)
, m_pLight(NULL)
{
	m_eStatus = GS_LOBBY_;

}

KBOOL Kylin::ClLobby::Initialize()
{
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	//////////////////////////////////////////////////////////////////////////
	OgreRoot::GetSingletonPtr()->GetMainWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);

	m_pLight = OgreRoot::GetSingletonPtr()->GetSceneManager()->createLight("LobbyLight");
	m_pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	Ogre::Vector3 vec(-0.3, -0.3, -0.3);
	vec.normalise();

	m_pNode = OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();

	Ogre::MeshManager::getSingleton().createPlane(
		"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* pEntity = OgreRoot::GetSingletonPtr()->GetSceneManager()->createEntity("FloorPlane", "FloorPlane");
	pEntity->setMaterialName("Ground");
	m_pNode->attachObject(pEntity);
	
	OgreRoot::GetSingletonPtr()->GetCameraController()->SetTarget(m_pNode);
	OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("FirstPerson");
	
	return true;
}

KVOID Kylin::ClLobby::Tick( KFLOAT fElapsed )
{

}

KVOID Kylin::ClLobby::Destroy()
{
	OgreRoot::GetSingletonPtr()->GetCameraController()->GetMode("FirstPerson")->stop();

	OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyLight(m_pLight);

	m_pNode->removeAndDestroyAllChildren();
	OgreRoot::GetSingletonPtr()->GetSceneManager()->destroySceneNode(m_pNode);
	m_pNode = NULL;
}

KVOID Kylin::ClLobby::UiLoader()
{
	Kylin::LobbyMenu* pLobby = KNEW Kylin::LobbyMenu();
	pLobby->Initialize();	

	//////////////////////////////////////////////////////////////////////////
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->RegisterGui(pLobby);
}

