#include "cltpch.h"
#include "ClSceneLoader.h"
#include "RegisterClass.h"
#include "Property.h"
#include "rOgreRoot.h"
#include "CameraControl.h"
#include "GameStatus.h"
#include "kylinroot.h"
#include "KylinHelper.h"
#include "Character.h"
#include "ClActionFactory.h"
#include "PlayerController.h"
#include "Scene.h"


Kylin::ClSceneLoader::ClSceneLoader()
: SceneLoader()
, m_pController(NULL)
{

}

KBOOL Kylin::ClSceneLoader::LoadPlayer()
{
	PropertySet kProp;
	KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Deserialize(kProp);
	KUINT uGid;
	if (!kProp.GetUIntValue("$PlayerID",uGid))
	{
		assert(!"无法获得角色ID");
		return false;
	}
	
	// 创建角色
	Kylin::Entity * pMyself = KylinHelper::GetSingletonPtr()->SpawnCharactor(uGid,id_character);
	if (pMyself)
	{
		//OgreRoot::GetSingletonPtr()->GetCameraController()->SetTarget(pMyself->GetSceneNode());
		//OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("ChasePerson");

		Character* pChar = BtDynamicCast(Character,pMyself);
		SAFE_CALL(pChar,SetActionFactory(KNEW ClActionFactory(pChar->GetActionDispatcher())));

		//-------------------------------------------------
		// 获得出生位置及出生朝向
		KANY var;
		if (kProp.GetValue("$BirthPosition",var))
		{
			KPoint3 kPos = boost::any_cast<KPoint3>(var);
			SAFE_CALL(pChar,SetTranslateToTerrain(kPos));
		}
		if (kProp.GetValue("$BirthRotation",var))
		{
			KQuaternion kQua = boost::any_cast<KQuaternion>(var);
			SAFE_CALL(pChar,SetRotation(kQua));
		}
		
		//-------------------------------------------------
		// 设置角色控制器
		m_pController = KNEW PlayerController();
		m_pController->SetTarget(pChar);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// test code, when mulit-player to set local player

	return true;
}

KVOID Kylin::ClSceneLoader::LoadLevel()
{
	PropertySet kProp;

	kProp.SetValue("$CLASS_ID",(KUINT)id_level);
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

	SceneLoader::Unload(pHag);
}
