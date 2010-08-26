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
#include "GuiManager.h"
#include "uiCursorEx.h"
#include "Character.h"
#include "OgreOggSound.h"


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


KBOOL Kylin::KylinRoot::GetTerrainHeight( KPoint3& vRetPos )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		Ogre::TerrainGroup* pTerrain = pStatus->m_pWorldManager->m_pActiveScene->m_pSceneLoader->getTerrainGroup();

		vRetPos.y = pTerrain->getHeightAtWorldPosition(vRetPos);

		return true;
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
Kylin::Entity* Kylin::KylinRoot::SpawnCharactor( KUINT uGid )
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

	dbItem.QueryField("TYPE",dbField);
	KSTR sCid  = boost::any_cast<KSTR>(dbField.m_aValue);
	KUINT uCid = (KUINT)OgreRoot::GetSingletonPtr()->GetScriptVM()->GetGlobalNumber(sCid.data());

	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);

 	dbItem.QueryField("NAME",dbField);
 	KSTR sName = boost::any_cast<KSTR>(dbField.m_aValue);

	dbItem.QueryField("HP",dbField);
	KINT nHP = boost::any_cast<KINT>(dbField.m_aValue);

	dbItem.QueryField("STR",dbField);
	KINT nSTR = boost::any_cast<KINT>(dbField.m_aValue);
	KINT nDef = 0;
	if (nSTR > 0)
		nDef = log10((KFLOAT)nSTR) / log10(2.0f);

	dbItem.QueryField("SPEED",dbField);
	KFLOAT fSpeed = boost::any_cast<KFLOAT>(dbField.m_aValue);
	
	dbItem.QueryField("CAMP",dbField);
	KINT nCamp = boost::any_cast<KINT>(dbField.m_aValue);

	dbItem.QueryField("EXP",dbField);
	KINT nExp = boost::any_cast<KINT>(dbField.m_aValue);

	// ע�� ·��ǰ������ "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	OgreUtils::DynamicLoadMesh(sModel);
	
	//---------------------------------------------------------------
	// ִ�нŲ��ļ�
	KSTR sScript = "./Data/script/charactor/char_";
	sScript += Ogre::StringConverter::toString(uGid);
	sScript += ".lua";
	if (FileUtils::IsFileExist(sScript))
	{
		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFile(sScript.data());
	}
	//////////////////////////////////////////////////////////////////////////
	KSTR sMeshName = FileUtils::GetFileNameWithSuffix(sModel);

	PropertySet kProp;
	kProp.SetValue("$CLASS_ID",uCid);
	kProp.SetValue("$Mesh",sMeshName);

	kProp.SetValue("$GID",uGid);
	kProp.SetValue("$Shadows",true);
	kProp.SetValue("$HP",nHP);
	kProp.SetValue("$InitHP",nHP);
	kProp.SetValue("$STR", nSTR);
	kProp.SetValue("$DEF",nDef);
	kProp.SetValue("$Collision",true);
	kProp.SetValue("$Level", (KINT)1);
	kProp.SetValue("$Exp",nExp);
	kProp.SetValue("$Speed",fSpeed);
	kProp.SetValue("$Camp",nCamp);

	Entity * pEnt = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	SAFE_CALL(pEnt,SetName(sName));
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

KVOID Kylin::KylinRoot::SetMousePointer( KUINT uType )
{
	CursorEx* pCursor = GET_GUI_PTR(CursorEx);
	SAFE_CALL(pCursor,SetPointer(CursorEx::CursorType(uType)));
}	

KVOID Kylin::KylinRoot::DebugShowBoundingBox( KBOOL bFlag )
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		EntityManager::EntityPool kEntPool;
		pStatus->m_pWorldManager->m_pActiveScene->m_pEntityManager->FillEntityPool(kEntPool);
		for (KUINT i = 0; i < kEntPool.size(); i++)
		{
			kEntPool[i]->ShowBoundingBox(bFlag);
		}
	}
}

KFLOAT Kylin::KylinRoot::GetGameTime()
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		
		return pStatus->GetGameTime();
	}
	
	return .0f;
}

KUINT Kylin::KylinRoot::CheckRelation( Kylin::Character* pEnt1,Kylin::Character* pEnt2 )
{
	Assert(pEnt1);
	Assert(pEnt2);
	
	if (!pEnt1 || !pEnt2)
		return RELATION_FRIEND;

	KINT nCamp1 = 0,nCamp2 = 0;
	pEnt1->GetPropertyRef().GetIntValue("$Camp",nCamp1);
	pEnt2->GetPropertyRef().GetIntValue("$Camp",nCamp2);

	if (nCamp1 == nCamp2)
		return RELATION_FRIEND;
	
	if (pEnt1->IsDead() || pEnt2->IsDead())
		return RELATION_FRIEND;

	return RELATION_ENEMY;
}

Kylin::Entity* Kylin::KylinRoot::SpawnItem( KUINT uGid,KUINT uCid )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("ITEM_DB",sValue))
		return NULL;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// ��ѯ��Ӧ�Ľ�ɫ��Ϣ
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGid,dbItem))
		return false;

	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);

	dbItem.QueryField("TYPE",dbField);
	KSTR sType = boost::any_cast<KSTR>(dbField.m_aValue);
	KUINT uType = (KUINT)OgreRoot::GetSingletonPtr()->GetScriptVM()->GetGlobalNumber(sType.data());

	dbItem.QueryField("BELONG",dbField);
	KINT nBelong = boost::any_cast<KINT>(dbField.m_aValue);

	dbItem.QueryField("ICON",dbField);
	KSTR sIcon = boost::any_cast<KSTR>(dbField.m_aValue);

	dbItem.QueryField("EXPLAIN",dbField);
	KSTR sExplain = boost::any_cast<KSTR>(dbField.m_aValue);

	// ע�� ·��ǰ������ "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	OgreUtils::DynamicLoadMesh(sModel);

	//////////////////////////////////////////////////////////////////////////
	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);

	PropertySet kProp;

	// ������Ч����
	if (dbItem.QueryField("EFFECT",dbField))
	{
		KINT nEffectID = boost::any_cast<KINT>(dbField.m_aValue);	
		KANY aRet;
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TEMPLATE",aRet) )
		{
			KSTR sEffect = boost::any_cast<KSTR>(aRet);	
			kProp.SetValue("$SpawnEffect",sEffect);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"SCALE",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kProp.SetValue("$SpawnEffectScale",fScale);
		}
	}

	kProp.SetValue("$CLASS_ID",(KUINT)uCid);
	kProp.SetValue("$Mesh",sName);
	kProp.SetValue("$GID",uGid);
	kProp.SetValue("$Type",uType);
	kProp.SetValue("$Belong",(KUINT)nBelong);
	kProp.SetValue("$Icon",sIcon);
	kProp.SetValue("$Explain", sExplain);

	Entity * pEnt = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	//////////////////////////////////////////////////////////////////////////

	return pEnt;
}

KVOID Kylin::KylinRoot::ClearMessage()
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		pStatus->m_pWorldManager->m_pActiveScene->m_pEventManager->RemoveAllEvents();
	}
}

KVOID Kylin::KylinRoot::QuitGame()
{
	if (GetGameFramePtr()->m_pActiveStatus->m_eStatus == GS_GAME_)
	{
		Kylin::GSGame* pStatus = static_cast<Kylin::GSGame*>(GetGameFramePtr()->m_pActiveStatus);
		pStatus->Quit();
	}
}

OgreOggSound::OgreOggISound* Kylin::KylinRoot::CreateSound( KCSTR& sName, KINT nID )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("SOUND_DB",sValue))
		return NULL;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// ��ѯ��Ӧ�Ľ�ɫ��Ϣ
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(nID,dbItem))
		return NULL;

	DataItem::DataField dbField;
	dbItem.QueryField("NAME",dbField);
	KSTR sFile = boost::any_cast<KSTR>(dbField.m_aValue);
	
	dbItem.QueryField("LOOP",dbField);
	KBOOL bLoop = boost::any_cast<KBOOL>(dbField.m_aValue);
	
	OgreOggSound::OgreOggISound* pSound = NULL;
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr()->hasSound(sName))
	{
		pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getSound(sName);
	}
	else
	{
		pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->createSound(sName,sFile,false,bLoop);
	}

	if (pSound)
		pSound->play();

	return pSound;
}

KBOOL Kylin::KylinRoot::CreateSound( KCSTR& sName, KINT nID, const KPoint3& pt, KFLOAT fDis )
{
	OgreOggSound::OgreOggISound* pSound = CreateSound(sName,nID);

	if (pSound)
	{
		pSound->setPosition(pt);
		pSound->setMaxDistance(fDis);
		pSound->disable3D(false);

		return true;
	}

	return false;
}

KVOID Kylin::KylinRoot::PlaySound( KCSTR& sName )
{
	OgreOggSound::OgreOggISound* pSound = NULL;
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr()->hasSound(sName))
	{
		pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getSound(sName);
		pSound->play();
	}
}

KVOID Kylin::KylinRoot::StopSound( KCSTR& sName )
{
	OgreOggSound::OgreOggISound* pSound = NULL;
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr()->hasSound(sName))
	{
		pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getSound(sName);
		pSound->stop();
	}
}

KVOID Kylin::KylinRoot::DestroySound( KCSTR& sName )
{
	OgreOggSound::OgreOggISound* pSound = NULL;
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr()->hasSound(sName))
	{
		OgreOggSound::OgreOggSoundManager::getSingletonPtr()->destroySound(sName);
	}
}

KBOOL Kylin::KylinRoot::IsPlaying( KCSTR& sName )
{
	OgreOggSound::OgreOggISound* pSound = NULL;
	if (OgreOggSound::OgreOggSoundManager::getSingletonPtr()->hasSound(sName))
	{
		pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getSound(sName);
		return pSound->isPlaying();
	}

	return false;
}

KVOID Kylin::KylinRoot::ChangeBgSound( KINT nNewID )
{
	OgreOggSound::OgreOggISound* pSound = OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getSound("$BackgroundSound");
	if (pSound)
	{
		// Get SceneManager
		Ogre::SceneManager* s = pSound->getSceneManager();
		s->destroyMovableObject(pSound);
	}

	DestroySound("$BackgroundSound");
	
	CreateSound("$BackgroundSound",nNewID);
}
