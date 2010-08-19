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
#include "StringUtils.h"
#include "ScriptVM.h"
#include "XmlStream.h"
#include "OgreOggSound.h"


Kylin::SceneHag::SceneHag()
{
	m_uSceneID	= INVALID_ID;
	m_uType		= INVALID_ID;
	m_uLevel	= INVALID_ID;
	m_sName		= "";
	m_nBgSound	= -1;
	m_sSceneFile= "";
	m_bPass		= FALSE;
}


Kylin::Scene::Scene( const SceneHag& kSceneHag )
: m_kSceneHag(kSceneHag)
, m_pSceneLoader(NULL)
{
	m_pEntityManager = KNEW EntityManager();
	m_pEventManager  = KNEW EventManager(m_pEntityManager);

}

Kylin::Scene::~Scene()
{
	SAFE_DEL(m_pEventManager);
	SAFE_DEL(m_pEntityManager);
}

KVOID Kylin::Scene::EnterScene( KVOID )
{
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ�����ļ�
	// ���س���
	SpawnScene();
	//////////////////////////////////////////////////////////////////////////
}

KVOID Kylin::Scene::LeaveScene( KVOID )
{
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr())
	{
		OgreOggSound::OgreOggSoundManager::getSingletonPtr()->stopAllSounds();	
		
		//OgreOggSound::OgreOggSoundManager::getSingletonPtr()->destroyAllSounds();	
	}

	m_pEntityManager->Destroy();
	m_pEventManager->RemoveAllEvents();

	SAFE_CALL(m_pSceneLoader,Unload(&m_kSceneHag));
	SAFE_DEL(m_pSceneLoader);

	if (PhyX::PhysicalSystem::Initialized())
	{
		PhyX::PhysicalSystem::GetSingletonPtr()->Clear();
	}
}

KVOID Kylin::Scene::SpawnScene()
{
	Assert(KylinRoot::GetSingletonPtr()->GetGameFramePtr());
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();
	
	//-----------------------------------------------------------------
	// ���س���
	Ogre::FileInfoListPtr resPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("General", m_kSceneHag.m_sSceneFile);
	Ogre::FileInfo fInfo = (*(resPtr->begin()));

	KSTR sName = fInfo.archive->getName();
	sName += "/" + m_kSceneHag.m_sSceneFile;
	//------------------------------------------------------------------
	// ���ر�������
	
	KylinRoot::GetSingletonPtr()->CreateSound("$BackgroundSound",m_kSceneHag.m_nBgSound);

	//------------------------------------------------------------------
	// ִ��lua�ļ�
	KSTR sLua = StringUtils::replace(sName,".xml",".lua");
	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFile(sLua.data());
	//-----------------------------------------------------------------
	// ����level
	m_pSceneLoader->LoadLevel(m_kSceneHag.m_sSceneFile);
	//-----------------------------------------------------------------
	// ���س�����NPC��
	if (!m_pSceneLoader->LoadScene(m_kSceneHag.m_sSceneFile))
	{
		AssertEx(NULL,"��������ʧ�ܣ�");
		return;
	}
	//-----------------------------------------------------------------
	// �������
	m_pSceneLoader->LoadPlayer();
}

KVOID Kylin::Scene::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pEventManager,HandleEvents(fElapsed));
	SAFE_CALL(m_pEntityManager,Tick(fElapsed));	
	
	if (PhyX::PhysicalSystem::Initialized())
		PhyX::PhysicalSystem::GetSingletonPtr()->Tick(fElapsed);

	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr())
		OgreOggSound::OgreOggSoundManager::getSingletonPtr()->update(fElapsed);

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