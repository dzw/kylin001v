#include "cltpch.h"
#include "PlayerController.h"

#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "rOgreUtils.h"
#include "KylinRoot.h"

#include "Character.h"
#include "GameCamera.h"
#include "ActionDispatcher.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "ScriptVM.h"
#include "EffectDecal.h"



Kylin::PlayerController::PlayerController()
: InputListener()
, m_pHost(NULL)
, m_bCanRot(false)
, m_bMoveForward(true)
, m_kKeyDirection(KPoint3::ZERO)
, m_kMousePickPos(KPoint3::ZERO)
, m_pGuideEffect(NULL)
, m_pFocusEffect(NULL)
, m_pFocusEntity(NULL)
{
	//-----------------------------------------------------
	m_pCamera = KNEW GameCamera( OgreRoot::GetSingletonPtr()->GetCamera("$MainCamera"),
								 OgreRoot::GetSingletonPtr()->GetSceneManager()	);

	KylinRoot::GetSingletonPtr()->SetActiveCamera(m_pCamera);
	//-----------------------------------------------------
	// 贴花特效
	m_pGuideEffect = KNEW EffectDecal( OgreRoot::GetSingletonPtr()->GetSceneManager(),
		"Game/ReachAble",1 );
	m_pFocusEffect = KNEW EffectDecal( OgreRoot::GetSingletonPtr()->GetSceneManager(),
		"Game/UnitSelect",1 );
	//-----------------------------------------------------
	// 创建射线交集
	OgreRoot::GetSingletonPtr()->CreateSceneRay();
}


Kylin::PlayerController::~PlayerController()
{
	//-----------------------------------------------------
	// 销毁射线交集
	OgreRoot::GetSingletonPtr()->DestroySceneRay();
	//-----------------------------------------------------
	SAFE_DEL(m_pFocusEffect);
	SAFE_DEL(m_pGuideEffect);
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

	m_pCamera->Update(fElapsed);

	if (m_pFocusEntity && m_pFocusEffect->IsVisible())
		m_pFocusEffect->MoveTo(m_pFocusEntity->getParentSceneNode()->getPosition());
}

KVOID Kylin::PlayerController::UpdateBody( KFLOAT fElapsed )
{
	m_kGoalDirection = Vector3::ZERO;   // we will calculate this

	if (m_kKeyDirection != Vector3::ZERO)
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
		if (yawToGoal < 0) yawToGoal		= std::min<KFLOAT>(0, std::max<KFLOAT>(yawToGoal, yawAtSpeed)); 
		else if (yawToGoal > 0) yawToGoal	= std::max<KFLOAT>(0, std::min<KFLOAT>(yawToGoal, yawAtSpeed));

		m_pHost->GetSceneNode()->yaw(Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHost,KPoint3(0, 0, RUN_SPEED));
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
			if (m_fDistance < 3 && m_pGuideEffect->IsVisible())
				m_pGuideEffect->SetVisible(false);

			// move in current body direction (not the goal direction)
			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHost,KPoint3(0, 0, RUN_SPEED));
		}
	}
}

KVOID Kylin::PlayerController::OnMouseMove( KFLOAT fX, KFLOAT fY, KFLOAT fZ )
{
	if (OgreRoot::GetSingletonPtr()->GetMouseState().buttonDown(OIS::MB_Right))
	{
		m_pCamera->UpdateCameraGoal(-0.5f * fX, -0.5f * fY, -0.0005f * fZ);
	}
	else
		m_pCamera->UpdateCameraGoal(0, 0, -0.0005f * fZ);
}

KVOID Kylin::PlayerController::OnKeyUp( KUINT uKey )
{
	// keep track of the player's intended direction
	if (uKey == OIS::KC_W && m_kKeyDirection.z == -1) m_kKeyDirection.z = 0;
	else if (uKey == OIS::KC_A && m_kKeyDirection.x == -1) m_kKeyDirection.x = 0;
	else if (uKey == OIS::KC_S && m_kKeyDirection.z == 1) m_kKeyDirection.z = 0;
	else if (uKey == OIS::KC_D && m_kKeyDirection.x == 1) m_kKeyDirection.x = 0;

}

KVOID Kylin::PlayerController::OnKeyDown( KUINT uKey )
{
	// keep track of the player's intended direction
	if (uKey == OIS::KC_W) m_kKeyDirection.z = -1;
	else if (uKey == OIS::KC_A) m_kKeyDirection.x = -1;
	else if (uKey == OIS::KC_S) m_kKeyDirection.z = 1;
	else if (uKey == OIS::KC_D) m_kKeyDirection.x = 1;
	else if (uKey == OIS::KC_SPACE)
	{
		// jump if on ground
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHost,KPoint3(0, JUMP_ACCEL, 0));
	}

	if (m_kKeyDirection.z != 0 || m_kKeyDirection.x != 0)
	{
		KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHost,"do_walk");
		//-----------------------------------------------------------
		m_kMousePickPos = KPoint3::ZERO;
		m_pGuideEffect->SetVisible(false);
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

KVOID Kylin::PlayerController::OnLButtonDown( KINT nX, KINT nY )
{
// 	Ogre::Ray kRay;
// 	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
// 	{
// 		Ogre::Entity* pEnt = NULL;
// 		if ( OgreRoot::GetSingletonPtr()->PickOgreEntity(kRay,&pEnt, KylinRoot::KR_CHAR_MASK) )
// 		{
// 			// 如果选中的是自己
// 			if (pEnt && pEnt == m_pHost->GetEntityPtr())
// 			{	// 可以旋转摄像机
// 				m_bCanRot = true;
// 				// 改变鼠标
// 
// 				return;
// 			}
// 		}
// 		
// 		//////////////////////////////////////////////////////////////////////////
// 		KPoint3 vPos;
// 		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
// 		{
// 			SAFE_CALL(m_pHost->GetActionDispatcher(),SpawnAction(1,vPos));
// 		}
// 	}

	//////////////////////////////////////////////////////////////////////////
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 vPos;
		if (KylinRoot::GetSingletonPtr()->HitTest(kRay,vPos))
		{
			m_fDistance = (vPos - m_pHost->GetTranslate()).length();
			// 超过可视距离不可移动
			if ( m_fDistance > VISIBLE_DISTANCE )
			{
				// 提示声音
				return;
			}
			else
			{
				m_kMousePickPos = vPos;

				// 在选中位置播放动画
				m_pGuideEffect->MoveTo(vPos);
				m_pGuideEffect->SetVisible(true);
				
				// 播放角色动画
				KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHost,"do_walk");
			}

			//////////////////////////////////////////////////////////////////////////
			// 旋转到拾取的方向
			KPoint3 kDir = m_kMousePickPos - m_pHost->GetTranslate();		// B-A = A->B (see vector questions above)
			KPoint3 kSrc = m_pHost->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
			kSrc.y = 0;														// Ignore pitch difference angle
			kDir.y = 0;
			kSrc.normalise(); 

// 			if ((1.0f + kSrc.dotProduct(kDir)) < KZERO)						// Work around 180 degree quaternion rotation quirk                         
// 			{                                                                           
// 				m_pHost->GetSceneNode()->yaw(Ogre::Degree(180));                                                
// 			}                                                                           
// 			else                                                                        
			{                                                                           
				Quaternion kQuat = kSrc.getRotationTo(kDir);                        
				m_pHost->GetSceneNode()->rotate(kQuat);                                                    
			}
		}
	}
}

KVOID Kylin::PlayerController::OnRButtonDown( KINT nX, KINT nY )
{
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 kHit;
		Ogre::Entity* pEnt = NULL;
		if ( OgreUtils::PickEntity(kRay,&pEnt,kHit,KylinRoot::KR_SCENE_OBJ,VISIBLE_DISTANCE) )
		{
			m_pFocusEffect->MoveTo(pEnt->getParentSceneNode()->getPosition());
			m_pFocusEffect->SetVisible(true);
			m_pFocusEntity = pEnt;
		}
	}
}

KVOID Kylin::PlayerController::OnLButtonUp( KINT nX, KINT nY )
{
	if (m_bCanRot)
	{
		m_bCanRot = false;
		// 改变鼠标

	}
}