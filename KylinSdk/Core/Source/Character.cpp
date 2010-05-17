#include "corepch.h"
#include "Character.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"

#include "CollisionWrapper.h"
#include "ActionDispatcher.h"


#define CH_HEIGHT 1.0f;

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
: m_kDestination(KPoint3::ZERO)
, m_pActDispatcher(NULL)
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
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 vPos;
		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
		{
			SAFE_CALL(m_pActDispatcher,SpawnAction(1,vPos));
		}
	}

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
			m_kDestination = vPos;

			m_fDistance = (m_kDestination - GetTranslate()).length();
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

KVOID Kylin::Character::Tick( KFLOAT fElapsed )
{
	Entity::Tick(fElapsed);

	SAFE_CALL(m_pActDispatcher,Tick(fElapsed));

	UpdateMovement(fElapsed);
}

KVOID Kylin::Character::UpdateMovement( KFLOAT fElapsed )
{
	if (OgreRoot::GetSingletonPtr()->IsOpenConsole())
		return;

	// This segment of code will turn the avatar in the direction of travel
	KBOOL bForward	= OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_W) || 
						OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_UP);
	KBOOL bBack		= OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_S) || 
						OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_DOWN);
	KBOOL bLeft		= OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_A) || 
						OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_LEFT);
	KBOOL bRight	= OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_D) || 
						OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_RIGHT);

 	if(!(bForward || bBack || bLeft || bRight ))
 	{
// 		mVelocity = Ogre::Vector3::ZERO;
// 		if(!mIdle)
// 		{
// 			mWalkAS->setEnabled(false);
// 			mWalkAS->setWeight(0);
// 			mMoving = false;
// 
// 			mIdleAS->setEnabled(true);
// 			mIdleAS->setWeight(1);
// 			mCurrentAnimation = mIdleAS;
// 			mIdle = true;
// 		}
		if (m_kDestination != KPoint3::ZERO)
		{
			KPoint3 kDir = m_kDestination - GetTranslate();
			kDir.normalise();
			KFLOAT fOffset = fElapsed*17;
			m_fDistance -= fOffset;
			if (m_fDistance < 0.0001f)
			{				
				m_kDestination = KPoint3::ZERO;
			}
			else 
			{
				KPoint3 vPos(this->GetTranslate() + kDir*fOffset);
				vPos.y = 5000.0f;

				if (KylinRoot::GetSingletonPtr()->HitTest(vPos,Ogre::Vector3(Ogre::Vector3::NEGATIVE_UNIT_Y),vPos))
				{
					Ogre::Real r = m_pOgreNode->_getWorldAABB().getSize().y * KHALF;
					vPos.y += r;

					this->SetTranslate(vPos);
				}

				//m_pOgreNode->translate(kDir*fOffset,Ogre::Node::TS_LOCAL);
			}
		}

 		return;
 	}
// 	else
// 	{
// 		if(!mMoving)
// 		{
// 			mIdleAS->setEnabled(false);
// 			mIdleAS->setWeight(0);
// 			mIdle = false;
// 
// 			mWalkAS->setEnabled(true);
// 			mWalkAS->setWeight(1);
// 			mCurrentAnimation = mWalkAS;
// 			mMoving = true;
// 		}
// 	}
	
	m_kDestination = Vector3::ZERO;
	KPoint3 kKeyDirection = Vector3::ZERO;
	Ogre::Degree dYaw;

	// check for 1 key combinations
	if( bForward ) kKeyDirection.z = 1;
	else if( bBack ) kKeyDirection.z = -1;
	if( bLeft ) kKeyDirection.x = 1;
	else if( bRight ) kKeyDirection.x = -1;

	KPoint3 kGoalDirection = Vector3::ZERO;   // we will calculate this

	//m_pOgreNode->yaw(Degree(dYaw));

	// calculate actually goal direction in world based on player's key directions
	kGoalDirection += kKeyDirection.z * m_pOgreNode->getOrientation().zAxis();
	kGoalDirection += kKeyDirection.x * m_pOgreNode->getOrientation().xAxis();
	kGoalDirection.y = 0;
	kGoalDirection.normalise();

	//Quaternion toGoal = m_pOgreNode->getOrientation().zAxis().getRotationTo(kGoalDirection);
	// calculate how much the character has to turn to face goal direction
	//Real yawToGoal = toGoal.getYaw().valueDegrees();
	// this is how much the character CAN turn this frame
	//Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * fElapsed * 500;


	// turn as much as we can, but not more than we need to
	//if (yawToGoal < 0) yawToGoal =  Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
	//else if (yawToGoal > 0) yawToGoal =  Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);

	//m_pOgreNode->yaw(Degree(yawToGoal));

	// move in current body direction (not the goal direction)
	KPoint3 vPos(this->GetTranslate() + kGoalDirection*fElapsed*17);
	vPos.y = 5000.0f;

	if (KylinRoot::GetSingletonPtr()->HitTest(vPos,Ogre::Vector3(Ogre::Vector3::NEGATIVE_UNIT_Y),vPos))
	{
		Ogre::Real r = m_pOgreNode->_getWorldAABB().getSize().y * KHALF;
		vPos.y += r;

		this->SetTranslate(vPos);
	}
		//m_pOgreNode->translate(kGoalDirection*fElapsed*17,Ogre::Node::TS_LOCAL);
}

KBOOL Kylin::Character::Init( const PropertySet& kProp )
{
	if (!Entity::Init(kProp))
		return false;

	//mCharacter = new OgreOpcode::CharacterController(mCollisionContext,mAvatarBaseNode,0.3,10);
	//////////////////////////////////////////////////////////////////////////
	KPoint3 vPos(this->GetTranslate());
	vPos.y = 5000.0f;

	if (KylinRoot::GetSingletonPtr()->HitTest(vPos,Ogre::Vector3(Ogre::Vector3::NEGATIVE_UNIT_Y),vPos))
	{
		Ogre::Real r = m_pOgreNode->_getWorldAABB().getSize().y * KHALF;
		vPos.y += r;

		this->SetTranslate(vPos);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	m_pActDispatcher = KNEW ActionDispatcher(this->GetID());

	return true;
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

	Entity::Destroy();
}

Kylin::ActionDispatcher* Kylin::Character::GetActionDispatcher()
{
	return m_pActDispatcher;
}