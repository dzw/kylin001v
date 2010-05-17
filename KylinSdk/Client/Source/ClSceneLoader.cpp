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
	
	Kylin::Entity * pMyself = KylinHelper::GetSingletonPtr()->SpawnCharactor(uGid,id_character);
	if (pMyself)
	{
		OgreRoot::GetSingletonPtr()->GetCameraController()->SetTarget(pMyself->GetSceneNode());
		OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("ChasePerson");

		Character* pChar = BtDynamicCast(Character,pMyself);
		SAFE_CALL(pChar,SetActionFactory(KNEW ClActionFactory(pChar->GetActionDispatcher())));
	}

	return true;
}

KVOID Kylin::ClSceneLoader::LoadLevel()
{
	PropertySet kProp;

	kProp.SetValue("$CLASS_ID",(KUINT)id_level);
	KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
}