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
#include "Zone.h"
#include "GameStatus.h"


extern Kylin::AppFrame* g_theApp;

Kylin::Entity * Kylin::KylinRoot::GetEntity( KUINT uID )
{
	assert(GetGameFramePtr()->m_pActiveStatus);
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		return pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->GetEntityPtr(uID);
	}

	return NULL;
}

Kylin::GameFrame* Kylin::KylinRoot::GetGameFramePtr()
{
	return static_cast<Kylin::GameFrame*>(g_theApp);
}

KBOOL Kylin::KylinRoot::HitTest( const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vRetPos )
{
	Ogre::Ray kRay(vOrg,vDir);
	
	return HitTest(kRay,vRetPos);
}

KBOOL Kylin::KylinRoot::HitTest( const Ogre::Ray &kCamRay, KPoint3& vRetPos )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		Ogre::TerrainGroup* pTerrain = pStatus->m_pWorldManager->m_pActiveScene->m_pSceneLoader->getTerrainGroup();

		Ogre::TerrainGroup::RayResult result = pTerrain->rayIntersects(kCamRay);
		if(result.hit)
		{
			vRetPos = result.position;

			//if (pStatus->m_pWorldManager->m_pActiveScene->m_pZone->GetTile(vRetPos.x,vRetPos.z) > 0)
				return true;
		}
	}

	return false;
}

Kylin::Entity * Kylin::KylinRoot::SpawnEntity( PropertySet& rProp )
{
	KUINT uClassID;
	if (!rProp.GetUIntValue("$CLASS_ID",uClassID))
		return NULL;

	Entity* pEnt = NULL;
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);

		pEnt = pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->SpawnEntity(uClassID);
		assert(pEnt);
		if (pEnt)
		{
			if (!pEnt->Init(rProp))
			{
				pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->DestroyEntity(pEnt->GetID());
				return NULL;
			}
			//pEnt->PostSpawn();
		}	
	}

	return pEnt;
}

KVOID Kylin::KylinRoot::PostMessage( KUINT uEntID,const EventPtr spEvent )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		pStatus->m_pWorldManager->m_pActiveScene->m_pEventManager->SendEvent(uEntID,spEvent);
	}
}

KVOID Kylin::KylinRoot::SwitchStatus( GameStatus* pStatus )
{
	GetGameFramePtr()->SwitchStatus(pStatus);
}

Kylin::GameStatus* Kylin::KylinRoot::GetCurrentGameStatus()
{
	return GetGameFramePtr()->m_pActiveStatus;	
}

KVOID Kylin::KylinRoot::DebugHideEntities( KBOOL bFlag )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		EntityManager::EntityPool kEntPool;
		pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->FillEntityPool(kEntPool);
		for (KUINT i = 0; i < kEntPool.size(); i++)
		{
			kEntPool[i]->SetVisible(bFlag);
		}
	}
}

KVOID Kylin::KylinRoot::DebugHideTerrain( KBOOL bFlag )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		//pStatus->m_pWorldManager->m_pActiveScene->m_pSceneLoader->getTerrainGroup()->getTerrainIterator()
	}
}

KVOID Kylin::KylinRoot::DestroyEntity( KUINT uEntID )
{
	Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
	
	if (pStatus)
		pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->DestroyEntity(uEntID);
}

