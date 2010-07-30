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

	// 读取配置文件
	// 加载场景
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
	// 加载场景
	Ogre::FileInfoListPtr resPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("General", m_kSceneHag.m_sSceneFile);
	Ogre::FileInfo fInfo = (*(resPtr->begin()));

	KSTR sName = fInfo.archive->getName();
	sName += "/" + m_kSceneHag.m_sSceneFile;
	//------------------------------------------------------------------
	// 预加载资源
// 	KSTR sRes = StringUtils::replace(sName,".xml",".res");
// 	
// 	XmlStream kXml(sRes.data());
// 	if (!kXml.Open(XmlStream::Read))
// 		return ;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 加载所有场景配置
// 	KSTR sDir;
// 	KBOOL bScene = kXml.SetToFirstChild("dir");
// 	while (bScene)
// 	{
// 		sDir = kXml.GetString("");
// 		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");
// 
// 		bScene = kXml.SetToNextChild("dir");
// 	}
// 
// 	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
// 
// 	kXml.Close();
	//------------------------------------------------------------------
	// 执行lua文件
	KSTR sLua = StringUtils::replace(sName,".xml",".lua");
	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFile(sLua.data());
	//-----------------------------------------------------------------
	// 加载level
	m_pSceneLoader->LoadLevel(m_kSceneHag.m_sSceneFile);
	//-----------------------------------------------------------------
	// 加载场景，NPC等
	if (!m_pSceneLoader->LoadScene(m_kSceneHag.m_sSceneFile))
	{
		AssertEx(NULL,"场景加载失败！");
		return;
	}
	//-----------------------------------------------------------------
	// 加载玩家
	m_pSceneLoader->LoadPlayer();
	//-----------------------------------------------------------------
	m_pZone->Initialize(sName.data());
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