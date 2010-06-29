#include "corepch.h"
#include "Scene.h"
#include "KylinRoot.h"
#include "GameFrame.h"
#include "SceneLoader.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "RegisterClass.h"
#include "Property.h"
#include "rOgreRoot.h"
#include "CameraControl.h"
#include "Entity.h"
#include "Zone.h"
#include "rPhyXSystem.h"


Kylin::SceneHag::SceneHag()
{
	m_uSceneID	= INVALID_ID;
	m_uType		= INVALID_ID;
	m_uLevel	= INVALID_ID;
	m_sName		= "";
	m_sBackgroundSound = "";
	m_sSceneFile= "";
	m_bPass		= FALSE;
}


Kylin::Scene::Scene( const SceneHag& kSceneHag )
: m_kSceneHag(kSceneHag)
, m_pSceneLoader(NULL)
{
	m_pEntityManager = KNEW EntityManager();
	m_pEventManager  = KNEW EventManager(m_pEntityManager);

	m_pZone			 = KNEW Zone();
}

Kylin::Scene::~Scene()
{
	LeaveScene();

	SAFE_DEL(m_pZone);
	SAFE_DEL(m_pEventManager);
	SAFE_DEL(m_pEntityManager);
}

KVOID Kylin::Scene::EnterScene( KVOID )
{
	//////////////////////////////////////////////////////////////////////////
	if (!PhyX::PhysicalSystem::Initialized())
		KNEW PhyX::PhysicalSystem();

	// ��ȡ�����ļ�
	// ���س���
	SpawnScene();
	//////////////////////////////////////////////////////////////////////////
}

KVOID Kylin::Scene::LeaveScene( KVOID )
{
	SAFE_CALL(m_pSceneLoader,Unload(&m_kSceneHag));
	SAFE_DEL(m_pSceneLoader);

	if (PhyX::PhysicalSystem::Initialized())
		KDEL PhyX::PhysicalSystem::GetSingletonPtr();
}

KVOID Kylin::Scene::SpawnScene()
{
	Assert(KylinRoot::GetSingletonPtr()->GetGameFramePtr());
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();
	
	//-----------------------------------------------------------------
	// ���س���
	Ogre::FileInfoListPtr resPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("General", m_kSceneHag.m_sSceneFile);
	Ogre::FileInfo fInfo = (*(resPtr->begin()));

	Ogre::String fname = fInfo.archive->getName();
	fname += "/" + m_kSceneHag.m_sSceneFile;

	if (!m_pSceneLoader->LoadScene(m_kSceneHag.m_sSceneFile))
	{
		AssertEx(NULL,"��������ʧ�ܣ�");
		return;
	}
	//-----------------------------------------------------------------
	// �������
	m_pSceneLoader->LoadPlayer();
	//-----------------------------------------------------------------
	// ����NPC��
	m_pSceneLoader->LoadLevel();
	//-----------------------------------------------------------------
	m_pZone->Initialize(fname.data());
}

KVOID Kylin::Scene::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pEventManager,HandleEvents(fElapsed));
	SAFE_CALL(m_pEntityManager,Tick(fElapsed));	
	
	if (PhyX::PhysicalSystem::Initialized())
		PhyX::PhysicalSystem::GetSingletonPtr()->Tick(fElapsed);

	SAFE_CALL(m_pSceneLoader,Tick(fElapsed));
}

KBOOL Kylin::Scene::IsValidPosition( const KPoint2& fvPos )
{
	
	return true;
}

Kylin::SceneLoader* Kylin::Scene::GetSceneLoader()
{
	return m_pSceneLoader;
}