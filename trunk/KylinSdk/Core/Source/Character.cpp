#include "corepch.h"
#include "Character.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"

#include "CollisionWrapper.h"

namespace Kylin
{
	BtImplementRTTI(Character, Entity, id_character);

	Implement_Event_Handler(Character, Entity)
	{
		{&ev_post_picked,			&EV_Picked},
		{&ev_post_pick_terrain,		&EV_PickTerrain},
		{NULL, NULL}
	};
}

Kylin::Character::Character()
{

}

Kylin::Character::~Character()
{

}

KVOID Kylin::Character::OnLButtonDown( KINT nX, KINT nY )
{
	
// 	EventPtr spEV(
// 			new Event(
// 			&ev_post_picked, 
// 			Event::ev_timing, 
// 			1.0f, 
// 			1, 
// 			EventArg(nX)
// 			));
// 
// 	KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);

}

KVOID Kylin::Character::OnRButtonDown( KINT nX, KINT nY )
{
	EventPtr spEV(
		new Event(
		&ev_post_pick_terrain, 
		Event::ev_immediate, 
		0, 
		2, 
		EventArg(nX),
		EventArg(nY)
		));

	KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
}

KVOID Kylin::Character::EV_Picked( EventPtr spEV )
{
	
}

KVOID Kylin::Character::EV_PickTerrain( EventPtr spEV )
{
	KINT nX = boost::get<KINT>(spEV->args[0]);
	KINT nY = boost::get<KINT>(spEV->args[1]);
	
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 vPos;
		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
		{
			//this->SetTranslate(vPos);
		}
	}

	CollisionWrapper::GetSingletonPtr()->Query(kRay,1000);
	
}

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