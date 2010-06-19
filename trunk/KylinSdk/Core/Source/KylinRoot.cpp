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
#include "DataManager.h"
#include "DataItem.h"
#include "DataLoader.h"
#include "FileUtils.h"
#include "rOgreUtils.h"
#include "GameCamera.h"


extern Kylin::AppFrame* g_theApp;


Kylin::KylinRoot::KylinRoot()
: m_pCamera(NULL)
{

}

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
		Scene* pScene = pStatus->m_pWorldManager->m_pActiveScene;
		assert(pScene);
		Ogre::Terrain* pTerr = pScene->m_pSceneLoader->getTerrainGroup()->getTerrain(0,0); // Ĭ�ϵ�ͼ��0��0��
		if (pTerr)
		{
			pTerr->setVisibilityFlags(bFlag);
		}
	}
}

KVOID Kylin::KylinRoot::DestroyEntity( KUINT uEntID )
{
	Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
	
	if (pStatus)
		pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->DestroyEntity(uEntID);
}

//////////////////////////////////////////////////////////////////////////
Kylin::Entity* Kylin::KylinRoot::SpawnCharactor( KUINT uGid, ClassID uCid )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("CHAR_DB",sValue))
		return NULL;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// ��ѯ��Ӧ�Ľ�ɫ��Ϣ
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGid,dbItem))
		return false;

	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);

	// ע�� ·��ǰ������ "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	OgreUtils::DynamicLoadMesh(sModel);
	
	//---------------------------------------------------------------
	//
	KSTR sScript = "./Data/script/charactor/char_";
	sScript += Ogre::StringConverter::toString(uGid);
	sScript += ".lua";
	if (FileUtils::IsFileExist(sModel))
	{
		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFile(sScript.data());
	}
	//////////////////////////////////////////////////////////////////////////
	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);

	PropertySet kProp;
	kProp.SetValue("$CLASS_ID",(KUINT)uCid);
	kProp.SetValue("$Mesh",sName);
	kProp.SetValue("$Materials",sMaterials);
	kProp.SetValue("$GID",uGid);
	kProp.SetValue("$Shadows",true);
	//kProp.SetValue("$CLLSN_SHAPE",(KUINT)1);
	//kProp.SetValue("$CLLSN_TYPE", (KUINT)0);
	//kProp.SetValue("$COLLISION",false);

	Entity * pEnt = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	//////////////////////////////////////////////////////////////////////////

	return pEnt;
}
//-------------------------------------------------------------------
KVOID Kylin::KylinRoot::SetActiveCamera( GameCamera* pCam )
{
	m_pCamera = pCam;
}
//-------------------------------------------------------------------
Kylin::GameCamera* Kylin::KylinRoot::GetActiveCamera()
{
	return m_pCamera;
}
//-------------------------------------------------------------------
KVOID Kylin::KylinRoot::SwitchScene( KUINT uSceneID )
{
	Assert(GetGameFramePtr()->m_pActiveStatus);
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		return pStatus->m_pWorldManager->SwitchScene(uSceneID);
	}
}
//-------------------------------------------------------------------
KVOID Kylin::KylinRoot::NotifyScriptEntity( Kylin::Entity* pEnt, KCSTR& sFunc )
{
	// ���Ž�ɫ����
	KUINT uGID = -1;
	if ( pEnt->GetPropertyRef().GetUIntValue("$GID",uGID) )
	{
		KSTR sModule = "char_";
		sModule += Ogre::StringConverter::toString(uGID);

		KVEC<KCCHAR *> kModules;
		kModules.push_back(sModule.data());

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,sFunc.data(),true,"i",pEnt->GetID());
	}
}

Kylin::Scene* Kylin::KylinRoot::GetCurrentScene()
{
	Assert(GetGameFramePtr()->m_pActiveStatus);
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		return pStatus->m_pWorldManager->m_pActiveScene;
	}
	return NULL;
}