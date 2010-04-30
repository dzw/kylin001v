#include "corepch.h"
#include "KylinRoot.h"
#include "GameFrame.h"
#include "ScriptVM.h"
#include "WorldManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "Entity.h"


extern Kylin::AppFrame* g_theApp;

Kylin::Entity * Kylin::KylinRoot::GetEntity( KUINT uID )
{
	assert(GetGameFramePtr()->m_pWorldManager);
	assert(GetGameFramePtr()->m_pWorldManager->m_pActiveScene);

	return GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pEntityManager->GetEntityPtr(uID);
}

Kylin::GameFrame* Kylin::KylinRoot::GetGameFramePtr()
{
	return static_cast<Kylin::GameFrame*>(g_theApp);
}

KBOOL Kylin::KylinRoot::HitTest( const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vRetPos )
{
	assert(GetGameFramePtr()->m_pWorldManager->m_pActiveScene);
	assert(GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pSceneLoader);

	Ogre::Ray kRay(vOrg,vDir);
	
	return HitTest(kRay,vRetPos);
}

KBOOL Kylin::KylinRoot::HitTest( const Ogre::Ray &kCamRay, KPoint3& vRetPos )
{
	Ogre::TerrainGroup* pTerrain = GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pSceneLoader->getTerrainGroup();

	Ogre::TerrainGroup::RayResult result = pTerrain->rayIntersects(kCamRay);
	if(result.hit)
	{
		vRetPos = result.position;
		return true;
	}
	else
		return false;
}

Kylin::Entity * Kylin::KylinRoot::SpawnEntity( PropertySet& rProp )
{
	KUINT uClassID;
	if (!rProp.GetUIntValue("$CLASS_ID",uClassID))
		return NULL;

	assert(GetGameFramePtr()->m_pWorldManager->m_pActiveScene);

	Entity* pEnt = GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pEntityManager->SpawnEntity(uClassID);
	assert(pEnt);
	if (pEnt)
	{
		if (!pEnt->Init(rProp))
		{
			GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pEntityManager->DestroyEntity(pEnt->GetID());
			return NULL;
		}

		pEnt->PostSpawn();
	}	
	
	return pEnt;
}

KVOID Kylin::KylinRoot::PostMessage( KUINT uEntID,const EventPtr spEvent )
{
	assert(GetGameFramePtr()->m_pWorldManager->m_pActiveScene);

	GetGameFramePtr()->m_pWorldManager->m_pActiveScene->m_pEventManager->SendEvent(uEntID,spEvent);
}