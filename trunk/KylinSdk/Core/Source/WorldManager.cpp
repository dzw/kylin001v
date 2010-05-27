#include "corepch.h"
#include "WorldManager.h"
#include "Scene.h"


Kylin::WorldManager::WorldManager()
: m_pActiveScene(NULL)
{
	
}

Kylin::WorldManager::~WorldManager()
{

}


KBOOL Kylin::WorldManager::Initialize( KCCHAR* pWorldCfg )
{
	SceneHag* hag = new SceneHag;
	hag->m_sSceneFile = "level_em.xml";
	hag->m_uSceneID = 10000000;
	hag->m_sName = "111";
	hag->m_uType = 1;
	hag->m_bPass = false;
	hag->m_uLevel= 1;
	
	m_kSceneInfo.insert(std::pair<KUINT,SceneHag*>(hag->m_uSceneID,hag));
	
	//////////////////////////////////////////////////////////////////////////
	//
	EnterScene(10000000);

	return true;
}

KVOID Kylin::WorldManager::Destroy()
{
	KMAP<KUINT,SceneHag*>::iterator begin = m_kSceneInfo.begin();
	KMAP<KUINT,SceneHag*>::iterator end   = m_kSceneInfo.end();
	for (KMAP<KUINT,SceneHag*>::iterator it=begin; it != end; it++)
	{
		KDEL it->second;
	}
	m_kSceneInfo.clear();

	m_pActiveScene->LeaveScene();
	KDEL m_pActiveScene;
}

KBOOL Kylin::WorldManager::EnterScene( KINT nSceneID )
{
	KMAP<KUINT,SceneHag*>::iterator it = m_kSceneInfo.find(nSceneID);
	if (it != m_kSceneInfo.end())
	{
		m_pActiveScene = KNEW Scene(*(it->second));
		m_pActiveScene->EnterScene();
	}
	
	return true;
}

KVOID Kylin::WorldManager::ChangeScene( KINT idTargetScene, const KPoint2& fvPos, KINT nDirection, BYTE bFlag )
{
	m_pActiveScene->LeaveScene();
	KDEL m_pActiveScene;
	
	EnterScene(idTargetScene);
	m_pActiveScene->EnterScene();
}

Kylin::Scene* Kylin::WorldManager::GetActiveScene( KVOID )
{
	return m_pActiveScene;
}

KVOID Kylin::WorldManager::Tick( KFLOAT fElapsed )
{
	if (m_pActiveScene)
		m_pActiveScene->Tick(fElapsed);
}