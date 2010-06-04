#include "corepch.h"
#include "Character.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "ScriptVM.h"

#include "CollisionWrapper.h"
#include "ActionDispatcher.h"
#include "rOgreUtils.h"
#include "Avatar.h"

#define CH_HEIGHT 1.0f;

namespace Kylin
{
	BtImplementRTTI(Character, Entity, id_character);

	Implement_Event_Handler(Character, Entity)
	{
		//{&ev_post_picked,			&EV_Picked},
		//{&ev_post_pick_terrain,		&EV_PickTerrain},
		{NULL, NULL}
	};
}

Kylin::Character::Character()
: m_pActDispatcher(NULL)
, m_pAvatar(NULL)
{

}

// KVOID Kylin::Character::EV_Picked( EventPtr spEV )
// {
// 	
// }
// 
// KVOID Kylin::Character::EV_PickTerrain( EventPtr spEV )
// {
// 	KINT nX = boost::get<KINT>(spEV->args[0]);
// 	KINT nY = boost::get<KINT>(spEV->args[1]);
// 	
// 	Ogre::Ray kRay;
// 	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
// 	{
// 		KPoint3 vPos;
// 		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
// 		{
// 			//this->SetTranslate(vPos);
// 			m_kDestination = vPos;
// 
// 			m_fDistance = (m_kDestination - GetTranslate()).length();
// 		}
// 	}
// 
// 	CollisionWrapper::GetSingletonPtr()->Query(kRay,1000);
// 	
// }

KVOID Kylin::Character::OnEntityCllsn( Node* pCollidee,const KPoint3& rNormal )
{
	int i = 0;
}

KBOOL Kylin::Character::OnShouldCllsn( Node* pCollidee )
{
	if (this != pCollidee)
		return true;

	return false;
}

KVOID Kylin::Character::Tick( KFLOAT fElapsed )
{
	Entity::Tick(fElapsed);

	SAFE_CALL(m_pActDispatcher,Tick(fElapsed));
}

KBOOL Kylin::Character::Init( const PropertySet& kProp )
{
	if (!Entity::Init(kProp))
		return false;
	
	this->GetEntityPtr()->setQueryFlags(KylinRoot::KR_CHAR_MASK);
	//////////////////////////////////////////////////////////////////////////
	//SetTranslateToTerrain(this->GetTranslate());
	m_pAvatar = KNEW Avatar(this);
	//////////////////////////////////////////////////////////////////////////
	//
	m_pActDispatcher = KNEW ActionDispatcher(this->GetID());

	return true;
}


KVOID Kylin::Character::PostSpawn()
{
	Entity::PostSpawn();

	//////////////////////////////////////////////////////////////////////////
	KSTR sModule = "char_";
	sModule += "1"; // add gid

	KVEC<KCCHAR *> kModules;
	kModules.push_back(sModule.data());

	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"do_spawn",true,"i",this->GetID());

}

KVOID Kylin::Character::SetActionFactory( ActionFactory* pActFactory )
{
	assert(pActFactory);
	
	SAFE_CALL(m_pActDispatcher,SetFactory(pActFactory));
}

KVOID Kylin::Character::Destroy()
{
	m_pActDispatcher->DestroyAllAction();
	SAFE_DEL(m_pActDispatcher);
	
	SAFE_DEL(m_pAvatar);

	Entity::Destroy();
}

Kylin::ActionDispatcher* Kylin::Character::GetActionDispatcher()
{
	return m_pActDispatcher;
}

Kylin::Avatar* Kylin::Character::GetAvatar()
{
	return m_pAvatar;
}

// KVOID Kylin::Character::SetTranslateToTerrain( KPoint3 vPos )
// {
// 	vPos.y = 50000.0f;
// 	if (KylinRoot::GetSingletonPtr()->HitTest(vPos,Ogre::Vector3(Ogre::Vector3::NEGATIVE_UNIT_Y),vPos))
// 	{
// 		Ogre::Real r = OgreUtils::GetEntitySize(this->GetEntityPtr(),this->GetScale()).y * KHALF;
// 		vPos.y += r - KHALF;
// 
// 		this->SetTranslate(vPos);
// 	}
// }

