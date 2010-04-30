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
}

Kylin::Scene::~Scene()
{
	SAFE_DEL(m_pEventManager);
	SAFE_DEL(m_pEntityManager);
}

KVOID Kylin::Scene::EnterScene( KVOID )
{
	// 读取配置文件
	// 加载NPC
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();
	m_pSceneLoader->Load(m_kSceneHag.m_sSceneFile);

	//////////////////////////////////////////////////////////////////////////
	// spawn level
	PropertySet kProp;
	
	kProp.SetValue("$CLASS_ID",(KUINT)id_level);
	KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	//////////////////////////////////////////////////////////////////////////
	
	kProp.Clear();
	kProp.SetValue("$CLASS_ID",(KUINT)id_character);
	kProp.SetValue("$MESH","Ogre.mesh");
	kProp.SetValue("$MATERIALS","ogre.mat/SOLID/TEX/ogre.tex.jpg/VertCol");
	
	kProp.SetValue("$CLLSN_SHAPE",(KUINT)1);
	kProp.SetValue("$CLLSN_TYPE", (KUINT)0);
	kProp.SetValue("$COLLISION",true);

	Entity * pMyself = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	if (pMyself)
	{
		OgreRoot::GetSingletonPtr()->GetCameraController()->SetTarget(pMyself->GetSceneNode());
		OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("ChasePerson");
	}
}

KVOID Kylin::Scene::LeaveScene( KVOID )
{
	m_pSceneLoader->Unload(&m_kSceneHag);
	SAFE_DEL(m_pSceneLoader);
}

KVOID Kylin::Scene::SpawnEntities()
{
	
}

KVOID Kylin::Scene::Tick( KFLOAT fElapsed )
{
	m_pEventManager->HandleEvents(fElapsed);
	m_pEntityManager->Tick(fElapsed);	
}

KBOOL Kylin::Scene::IsValidPosition( const KPoint2& fvPos )
{
	
	return true;
}

KBOOL Kylin::Scene::CreateLevel( KCCHAR* pLevelName )
{
	
	return true;
}