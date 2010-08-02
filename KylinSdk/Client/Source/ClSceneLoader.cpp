#include "cltpch.h"
#include "ClSceneLoader.h"
#include "RegisterClass.h"
#include "ClRegisterClass.h"
#include "Property.h"
#include "rOgreRoot.h"
#include "CameraControl.h"
#include "GameStatus.h"
#include "kylinroot.h"
#include "Character.h"
#include "ClActionFactory.h"
#include "PlayerController.h"
#include "Scene.h"
#include "uiCharInfoMenu.h"
#include "StringUtils.h"
#include "PathwayLoader.h"


Kylin::ClSceneLoader::ClSceneLoader()
: SceneLoader()
, m_pController(NULL)
, m_pPathwayLoader(NULL)
{

}

KBOOL Kylin::ClSceneLoader::LoadPlayer()
{
	PropertySet kProp;
	KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Deserialize(kProp);
	KUINT uGid;
	if (!kProp.GetUIntValue("$PlayerID",uGid))
	{
#ifdef _DEBUG
		uGid = 1;
#else
		Ogre::LogManager::getSingleton().logMessage("[ClSceneLoader] �޷���ý�ɫID!");
		uGid = 1;
		//return false;
#endif
	}
	
	// ������ɫ
	Kylin::Entity * pMyself = KylinRoot::GetSingletonPtr()->SpawnCharactor(uGid);
	if (pMyself)
	{
		Character* pChar = BtDynamicCast(Character,pMyself);
		SAFE_CALL(pChar,SetActionFactory(KNEW ClActionFactory(pChar->GetActionDispatcher())));

		//-------------------------------------------------
		// ��ó���λ�ü���������
		KANY var;
		if (kProp.GetValue("$BirthPosition",var))
		{
			KPoint3 kPos = boost::any_cast<KPoint3>(var);
			SAFE_CALL(pChar,SetTranslate(kPos));

			SAFE_CALL(pChar,GetPropertyRef().SetValue("$BirthPosition",kPos));
		}
		if (kProp.GetValue("$BirthRotation",var))
		{
			KQuaternion kQua = boost::any_cast<KQuaternion>(var);
			SAFE_CALL(pChar,SetRotation(kQua));

			SAFE_CALL(pChar,GetPropertyRef().SetValue("$BirthRotation",kQua));
		}
		
		//-------------------------------------------------
		// ���ý�ɫ������
		m_pController = KNEW PlayerController();
		m_pController->SetTarget(pChar);
		//-------------------------------------------------
	}
	
	//////////////////////////////////////////////////////////////////////////
	// test code, when mulit-player to set local player
	// test spawn npc
// 	Kylin::Entity * pNpc = KylinRoot::GetSingletonPtr()->SpawnCharactor(2,id_npc);
// 	if (pNpc)
// 	{
// 		Character* pChar = BtDynamicCast(Character,pNpc);
// 		SAFE_CALL(pChar,SetActionFactory(KNEW ClActionFactory(pChar->GetActionDispatcher())));
// 
// 		pNpc->SetTranslate(KPoint3(pMyself->GetTranslate().x -1 , 0 , pMyself->GetTranslate().z -1));
// 	}
	//-------------------------------------------------
// 	Kylin::Entity * pSpawner = KylinRoot::GetSingletonPtr()->SpawnCharactor(6,id_spawner);
// 	if (pSpawner)
// 	{
// 		pSpawner->SetTranslate(KPoint3(1 , 0 , 1));
// 	}

	return true;
}

KVOID Kylin::ClSceneLoader::LoadLevel(KSTR sSceneName)
{
	//---------------------------------------------------------------
	// ����·������Ϣ
	m_pPathwayLoader = KNEW PathwayLoader();
	m_pPathwayLoader->Load(sSceneName.data());
	//---------------------------------------------------------------
	KSTR sName = StringUtils::replace(sSceneName,".xml","");

	PropertySet kProp;

	kProp.SetValue("$CLASS_ID",(KUINT)id_level_cl);
	kProp.SetValue("$Name",sName);

	KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
}

KVOID Kylin::ClSceneLoader::Tick( KFLOAT fElapsed )
{
	SceneLoader::Tick(fElapsed);

	SAFE_CALL(m_pController,Tick(fElapsed));
}

KVOID Kylin::ClSceneLoader::Unload( SceneHag* pHag )
{
	SAFE_DEL(m_pController);
	SAFE_DEL(m_pPathwayLoader);

	SceneLoader::Unload(pHag);
}

Kylin::PlayerController* Kylin::ClSceneLoader::GetController()
{	
	return m_pController;
}

Kylin::PathwayLoader* Kylin::ClSceneLoader::GetPathwayLoader()
{
	return m_pPathwayLoader;
}