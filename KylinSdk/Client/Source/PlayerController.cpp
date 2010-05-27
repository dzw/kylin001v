#include "cltpch.h"
#include "PlayerController.h"

#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"

#include "Character.h"
#include "GameCamera.h"
#include "ActionDispatcher.h"


Kylin::PlayerController::PlayerController()
: InputListener()
, m_pHost(NULL)
, m_fCurPosY(.0f)
, m_bMoveForward(true)
, m_kKeyDirection(KPoint3::ZERO)
, m_kMousePickPos(KPoint3::ZERO)
, m_fVerticalVelocity(.0f)
{
	//SetupAnimations();

	m_pCamera = KNEW GameCamera( OgreRoot::GetSingletonPtr()->GetCamera("$MainCamera"),
								 OgreRoot::GetSingletonPtr()->GetSceneManager()	);
}


Kylin::PlayerController::~PlayerController()
{
	m_pCamera->Destroy();
	SAFE_DEL(m_pCamera);
}


KVOID Kylin::PlayerController::SetTarget( Character* pHost )
{	
	assert(pHost);
	if (!pHost) return;

	m_pHost = pHost;
	m_pCamera->SetTarget(m_pHost->GetSceneNode());
}

KVOID Kylin::PlayerController::Tick( KFLOAT fElapsed )
{
	UpdateBody(fElapsed);
	//UpdateAnimations(fElapsed);

	m_pCamera->Update(fElapsed);
}


KVOID Kylin::PlayerController::SetupAnimations()
{
	// this is very important due to the nature of the exported animations
	m_pHost->GetEntityPtr()->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
	{"daiji", "walk", "PAO", "tiao"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		m_arAnims[i] = m_pHost->GetEntityPtr()->getAnimationState(animNames[i]);
		m_arAnims[i]->setLoop(true);
		m_bFadingIn[i] = false;
		m_bFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	//SetBaseAnimation(ANIM_IDLE_BASE);

}


KVOID Kylin::PlayerController::UpdateBody( KFLOAT fElapsed )
{
	m_kGoalDirection = Vector3::ZERO;   // we will calculate this

	if (m_kKeyDirection != Vector3::ZERO && m_uBaseAnimID != ANIM_JUMP_BASE)
	{
		// calculate actually goal direction in world based on player's key directions
		m_kGoalDirection += m_kKeyDirection.z * m_pCamera->GetCameraNode()->getOrientation().zAxis();
		m_kGoalDirection += m_kKeyDirection.x * m_pCamera->GetCameraNode()->getOrientation().xAxis();
		m_kGoalDirection.y = 0;
		m_kGoalDirection.normalise();

		Quaternion toGoal = m_pHost->GetSceneNode()->getOrientation().zAxis().getRotationTo(m_kGoalDirection);

		// calculate how much the character has to turn to face goal direction
		KFLOAT yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		KFLOAT yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * fElapsed * TURN_SPEED;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<KFLOAT>(0, std::max<KFLOAT>(yawToGoal, yawAtSpeed)); 
		else if (yawToGoal > 0) yawToGoal = std::max<KFLOAT>(0, std::min<KFLOAT>(yawToGoal, yawAtSpeed));

		m_pHost->GetSceneNode()->yaw(Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		m_pHost->GetSceneNode()->translate(0, 0, fElapsed * RUN_SPEED ,//* m_arAnims[m_uBaseAnimID]->getWeight(),
			Ogre::Node::TS_LOCAL);
		
		m_pHost->SetTranslateToTerrain(m_pHost->GetTranslate());
	}
	else if (m_kMousePickPos != KPoint3::ZERO)
	{
		KPoint3 kDir = m_kMousePickPos - m_pHost->GetTranslate();
		kDir.normalise();
		KFLOAT fOffset = fElapsed * RUN_SPEED;
		m_fDistance -= fOffset;
		if (m_fDistance < KZERO)
		{				
			m_kMousePickPos = KPoint3::ZERO;
		}
		else 
		{
			// move in current body direction (not the goal direction)
			m_pHost->GetSceneNode()->translate(0, 0, fOffset ,//* m_arAnims[m_uBaseAnimID]->getWeight(),
				Ogre::Node::TS_LOCAL);

			m_pHost->SetTranslateToTerrain(m_pHost->GetTranslate());
		}
	}

// 	if (m_uBaseAnimID == ANIM_JUMP_BASE)
// 	{
// 		KPoint3 pos = m_pHost->GetTranslate();
// 
// 		if(m_arAnims[ANIM_JUMP_BASE]->getLength() - m_arAnims[ANIM_JUMP_BASE]->getTimePosition() < 0.01f)
// 		{
// 			// if we've hit the ground, change to landing state
// 			pos.y = m_fCurPosY;
// 			m_pHost->SetTranslate(pos);
// 			SetBaseAnimation(ANIM_IDLE_BASE, true);
// 			m_fTimer = 0;
// 			
// 			SetPosY(0.0);
// 			//---------
// 		}
// 	}

}

KVOID Kylin::PlayerController::UpdateAnimations( KFLOAT fElapsed )
{
	KFLOAT baseAnimSpeed = 1;
	KFLOAT topAnimSpeed = 1;

	m_fTimer += fElapsed;

	if (m_uBaseAnimID != ANIM_NONE) m_arAnims[m_uBaseAnimID]->addTime(fElapsed * baseAnimSpeed);
	
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (m_bFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			KFLOAT newWeight = m_arAnims[i]->getWeight() + fElapsed * ANIM_FADE_SPEED;
			m_arAnims[i]->setWeight(Math::Clamp<KFLOAT>(newWeight, 0, 1));
			if (newWeight >= 1) m_bFadingIn[i] = false;
		}
		else if (m_bFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			KFLOAT newWeight = m_arAnims[i]->getWeight() - fElapsed * ANIM_FADE_SPEED;
			m_arAnims[i]->setWeight(Math::Clamp<KFLOAT>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				m_arAnims[i]->setEnabled(false);
				m_bFadingOut[i] = false;
			}
		}
	}
}

KVOID Kylin::PlayerController::OnMouseMove( KFLOAT fX, KFLOAT fY, KFLOAT fZ )
{
	m_pCamera->UpdateCameraGoal(-0.05f * fX, -0.05f * fY, -0.0005f * fZ);
}

KVOID Kylin::PlayerController::OnKeyUp( KUINT uKey )
{
	// keep track of the player's intended direction
	if (uKey == OIS::KC_W && m_kKeyDirection.z == -1) m_kKeyDirection.z = 0;
	else if (uKey == OIS::KC_A && m_kKeyDirection.x == -1) m_kKeyDirection.x = 0;
	else if (uKey == OIS::KC_S && m_kKeyDirection.z == 1) m_kKeyDirection.z = 0;
	else if (uKey == OIS::KC_D && m_kKeyDirection.x == 1) m_kKeyDirection.x = 0;

	if (m_kKeyDirection.isZeroLength() && m_uBaseAnimID == ANIM_WALK_BASE)
	{
		// stop running if already moving and the player doesn't want to move
		//SetBaseAnimation(ANIM_IDLE_BASE);
	}
}

KVOID Kylin::PlayerController::OnKeyDown( KUINT uKey )
{
	if (uKey == OIS::KC_1)   // switch  1
	{
		//changeCameraDistance(3);
	}
	if (uKey == OIS::KC_2)   // switch  1
	{
		//changeCameraDistance(-3);
	}
	// keep track of the player's intended direction
	else if (uKey == OIS::KC_W) m_kKeyDirection.z = -1;
	else if (uKey == OIS::KC_A) m_kKeyDirection.x = -1;
	else if (uKey == OIS::KC_S) m_kKeyDirection.z = 1;
	else if (uKey == OIS::KC_D) m_kKeyDirection.x = 1;
	else if (uKey == OIS::KC_SPACE)
	{
		// jump if on ground
		//SetBaseAnimation(ANIM_JUMP_BASE, true);
		SetPosY(m_pHost->GetTranslate().y);
		m_fTimer = 0;
	}

	if (!m_kKeyDirection.isZeroLength() && m_uBaseAnimID == ANIM_IDLE_BASE)
	{
		// start running if not already moving and the player wants to move
		//SetBaseAnimation(ANIM_WALK_BASE, true);
	}
}

//////////////////////////////////////////////////////////////////////////
KVOID Kylin::PlayerController::SetMoveForwardEnabled( KBOOL bState )
{
	m_bMoveForward = bState;
}

KBOOL Kylin::PlayerController::IsMoveForwardEnabled()
{
	return m_bMoveForward;
}	

KBOOL Kylin::PlayerController::IsMove()
{
	if (m_kKeyDirection != KPoint3::ZERO)
		return true;

	return false;
}

KVOID Kylin::PlayerController::SetPosY( KFLOAT fPosY )
{
	m_fCurPosY = fPosY;
}

KBOOL Kylin::PlayerController::GetPosY()
{
	return m_fCurPosY;
}

KUINT Kylin::PlayerController::GetCurState()
{
	return	m_uBaseAnimID;
}

void Kylin::PlayerController::SetBaseAnimation(AnimID id, KBOOL reset)
{
	if (m_uBaseAnimID >= 0 && m_uBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		m_bFadingIn[m_uBaseAnimID] = false;
		m_bFadingOut[m_uBaseAnimID] = true;
	}

	m_uBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		m_arAnims[id]->setEnabled(true);
		m_arAnims[id]->setWeight(0);
		m_bFadingOut[id] = false;
		m_bFadingIn[id] = true;
		if (reset) m_arAnims[id]->setTimePosition(0);
	}
}

void Kylin::PlayerController::SetTopAnimation(AnimID id, KBOOL reset)
{
	if (m_uTopAnimID >= 0 && m_uTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		m_bFadingIn[m_uTopAnimID] = false;
		m_bFadingOut[m_uTopAnimID] = true;
	}

	m_uTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		m_arAnims[id]->setEnabled(true);
		m_arAnims[id]->setWeight(0);
		m_bFadingOut[id] = false;
		m_bFadingIn[id] = true;
		if (reset) m_arAnims[id]->setTimePosition(0);
	}
}

KVOID Kylin::PlayerController::OnLButtonDown( KINT nX, KINT nY )
{
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 vPos;
		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
		{
			SAFE_CALL(m_pHost->GetActionDispatcher(),SpawnAction(1,vPos));
		}
	}
}

KVOID Kylin::PlayerController::OnRButtonDown( KINT nX, KINT nY )
{
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 vPos;
		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
		{
			m_kMousePickPos = vPos;
			m_fDistance = (m_kMousePickPos - m_pHost->GetTranslate()).length();
			
			//////////////////////////////////////////////////////////////////////////
			// 旋转到拾取的方向
			KPoint3 kDir = m_kMousePickPos - m_pHost->GetTranslate();		// B-A = A->B (see vector questions above)
			KPoint3 kSrc = m_pHost->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
			kSrc.y = 0;														// Ignore pitch difference angle
			kDir.y = 0;
			kSrc.normalise(); 

			if ((1.0f + kSrc.dotProduct(kDir)) < KZERO)						// Work around 180 degree quaternion rotation quirk                         
			{                                                                           
				m_pHost->GetSceneNode()->yaw(Ogre::Degree(180));                                                
			}                                                                           
			else                                                                        
			{                                                                           
				Quaternion kQuat = kSrc.getRotationTo(kDir);                        
				m_pHost->GetSceneNode()->rotate(kQuat);                                                    
			}
		}
	}
}