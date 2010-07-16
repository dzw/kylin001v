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
#include "uiCursorEx.h"
#include "uiMonsterInfoMenu.h"
#include "DataManager.h"
#include "Action.h"
#include "NpcObject.h"
#include "gamedefine.h"
#include "ItemEntity.h"


Kylin::PlayerController::PlayerController()
: InputListener()
, m_pHostChar(NULL)
, m_bMoveForward(true)
, m_kKeyDirection(KPoint3::ZERO)
, m_kMousePickPos(KPoint3::ZERO)
, m_pGuideEffect(NULL)
, m_pFocusEffect(NULL)
, m_uTargetID(INVALID_ID)
, m_uDefaultActionID(INVALID_ID)
{
	//-----------------------------------------------------
	m_pCamera = KNEW GameCamera( OgreRoot::GetSingletonPtr()->GetCamera("$MainCamera"),
								 OgreRoot::GetSingletonPtr()->GetSceneManager()	);

	KylinRoot::GetSingletonPtr()->SetActiveCamera(m_pCamera);
	//-----------------------------------------------------
	// ������Ч
	m_pGuideEffect = KNEW EffectDecal( OgreRoot::GetSingletonPtr()->GetSceneManager(),
		"Game/ReachAble",1 );
	m_pFocusEffect = KNEW EffectDecal( OgreRoot::GetSingletonPtr()->GetSceneManager(),
		"Game/UnitSelect",1 );
	//-----------------------------------------------------
	// �������߽���
	OgreRoot::GetSingletonPtr()->CreateSceneQuery();

	//-----------------------------------------------------
}


Kylin::PlayerController::~PlayerController()
{
	//-----------------------------------------------------
	// �������߽���
	OgreRoot::GetSingletonPtr()->DestroyQuery();
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

	m_pHostChar = pHost;
	m_pCamera->SetTarget(m_pHostChar->GetSceneNode());
}

KVOID Kylin::PlayerController::Tick( KFLOAT fElapsed )
{
	UpdateBody(fElapsed);

	SAFE_CALL(m_pCamera,Update(fElapsed));

	UpdateEffect(fElapsed);
}

KVOID Kylin::PlayerController::UpdateBody( KFLOAT fElapsed )
{
	m_kGoalDirection = KPoint3::ZERO;   // we will calculate this

	if (m_kKeyDirection != KPoint3::ZERO)
	{
		// calculate actually goal direction in world based on player's key directions
		m_kGoalDirection += m_kKeyDirection.z * m_pCamera->GetCameraNode()->getOrientation().zAxis();
		m_kGoalDirection += m_kKeyDirection.x * m_pCamera->GetCameraNode()->getOrientation().xAxis();
		m_kGoalDirection.y = 0;
		m_kGoalDirection.normalise();

		Ogre::Quaternion toGoal = m_pHostChar->GetSceneNode()->getOrientation().zAxis().getRotationTo(m_kGoalDirection);

		// calculate how much the character has to turn to face goal direction
		KFLOAT yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		KFLOAT yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * fElapsed * TURN_SPEED;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal		= std::min<KFLOAT>(0, std::max<KFLOAT>(yawToGoal, yawAtSpeed)); 
		else if (yawToGoal > 0) yawToGoal	= std::max<KFLOAT>(0, std::min<KFLOAT>(yawToGoal, yawAtSpeed));

		m_pHostChar->GetSceneNode()->yaw(Ogre::Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(0, 0, RUN_SPEED));
	}
	else if (m_kMousePickPos != KPoint3::ZERO)
	{
		KPoint3 kDir = m_kMousePickPos - m_pHostChar->GetTranslate();
		kDir.normalise();
		KFLOAT fOffset = fElapsed * RUN_SPEED;
		m_fDistance -= fOffset;
		if (m_fDistance < KZERO)
		{				
			m_kMousePickPos = KPoint3::ZERO;
			if (m_kSelectAction.uActionGID != INVALID_ID)
			{
				SAFE_CALL(m_pHostChar->GetActionDispatcher(),Fire(m_kSelectAction.uActionGID,m_uTargetID));
				m_kSelectAction.Reset();
			}
		}
		else 
		{
			if (m_fDistance < 3 && m_pGuideEffect->IsVisible())
				m_pGuideEffect->SetVisible(false);

			// move in current body direction (not the goal direction)
			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(0, 0, RUN_SPEED));
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
//---------------------------------------------------------------
KVOID Kylin::PlayerController::OnMouseMove(KINT nX, KINT nY)
{
	if (m_pCamera->GetMode() == GameCamera::CM_FREE)
		return;

	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		Ogre::Entity* pEnt = NULL;
		KPoint3 kHit;
		if ( OgreUtils::PickEntityBoundBox(kRay,&pEnt,kHit,KylinRoot::KR_NPC_MASK,CLICK_DISTANCE) )
		{
			if (pEnt && !pEnt->getUserAny().isEmpty())
			{
				KUINT uID = Ogre::any_cast<KUINT>(pEnt->getUserAny());
				Character* pTar = BtDynamicCast(Character,KylinRoot::GetSingletonPtr()->GetEntity(uID));

				if (pTar && KylinRoot::GetSingletonPtr()->CheckRelation(m_pHostChar,pTar) == KylinRoot::RELATION_ENEMY)
				{
					// ���Ѿ��ı����ָ��
					KylinRoot::GetSingletonPtr()->SetMousePointer(CursorEx::CT_ATTACK);
				}
			}
		}
		else if (OgreUtils::PickEntityBoundBox(kRay,&pEnt,kHit,KylinRoot::KR_ITEM_MASK,CLICK_DISTANCE))
		{
			KylinRoot::GetSingletonPtr()->SetMousePointer(CursorEx::CT_PICKUP);
		}
		else
			KylinRoot::GetSingletonPtr()->SetMousePointer(CursorEx::CT_NORMAL);
	}
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
	if (m_pCamera->GetMode() != GameCamera::CM_FREE)
	{
		// keep track of the player's intended direction
		if (uKey == OIS::KC_W) m_kKeyDirection.z = -1;
		else if (uKey == OIS::KC_A) m_kKeyDirection.x = -1;
		else if (uKey == OIS::KC_S) m_kKeyDirection.z = 1;
		else if (uKey == OIS::KC_D) m_kKeyDirection.x = 1;
		else if (uKey == OIS::KC_SPACE)
		{
			// jump if on ground
			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(0, JUMP_ACCEL, 0));
		}

		if (m_kKeyDirection.z != 0 || m_kKeyDirection.x != 0)
		{
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHostChar,"do_walk");
			//-----------------------------------------------------------
			m_kMousePickPos = KPoint3::ZERO;
			m_pGuideEffect->SetVisible(false);
		}
	}
	else 
	{
		if (uKey == OIS::KC_W) m_kKeyDirection.z = -1;
		else if (uKey == OIS::KC_A) m_kKeyDirection.x = -1;
		else if (uKey == OIS::KC_S) m_kKeyDirection.z = 1;
		else if (uKey == OIS::KC_D) m_kKeyDirection.x = 1;
		else if (uKey == OIS::KC_E) m_kKeyDirection.y = 1;
		else if (uKey == OIS::KC_Q) m_kKeyDirection.y = -1;
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
	if (m_pCamera->GetMode() == GameCamera::CM_FREE)
		return;

	//////////////////////////////////////////////////////////////////////////
	Ogre::Ray kRay;
	OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay);
	{
		if (!SelectedEntity(kRay))
			SelectedTerrain(kRay);
	}
}

KVOID Kylin::PlayerController::OnRButtonDown( KINT nX, KINT nY )
{
	if (m_pCamera->GetMode() == GameCamera::CM_FREE)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	Ogre::Ray kRay;
	if (OgreRoot::GetSingletonPtr()->GetMouseRay(KPoint2(nX,nY),kRay))
	{
		KPoint3 kHit;
		Ogre::Entity* pEnt = NULL;
		if ( OgreUtils::PickEntity(kRay,&pEnt,kHit,KylinRoot::KR_NPC_MASK,CLICK_DISTANCE) )
		{
			KUINT uTargetID = Ogre::any_cast<KUINT>(pEnt->getUserAny());
			FocusTarget(uTargetID);
		}
		else if ( OgreUtils::PickEntity(kRay,&pEnt,kHit,KylinRoot::KR_ITEM_MASK,CLICK_DISTANCE) )
		{
			KUINT uTargetID = Ogre::any_cast<KUINT>(pEnt->getUserAny());
			Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(uTargetID);
			if (BtIsKindOf(ItemEntity,pEnt))
			{
				ItemEntity* pItem = BtStaticCast(ItemEntity,pEnt);
				pItem->BeCollected(m_pHostChar->GetID());
			}
		}
	}
}

KVOID Kylin::PlayerController::UseSkill( KUINT uActID )
{
	if (m_kSelectAction.uActionGID != INVALID_ID)
		return;

	KANY aValue;
	if ( DataManager::GetSingletonPtr()->Select("ACTION_DB",uActID,"TYPE",aValue) )
	{
		KSTR sValue = boost::any_cast<KSTR>(aValue);	
		
		ActionType type = Action::TransformType(sValue);

		if (type != AT_IMM)
		{
			m_kSelectAction.uActionGID = uActID;
			m_kSelectAction.uActionType= type;
		}
		
		if (type == AT_IMM)
		{
			SAFE_CALL(m_pHostChar->GetActionDispatcher(),Fire(uActID));
		}
		else if (type == AT_TAR)
		{
			// update cursor
		}
		else if (type == AT_POS)
		{
			// update cursor
		}
	}
	

	//------------------------------------------------------------------
	// m_bPrepareFire ��Ŀ�������ڹ�������ʱ����Ŀ���ƶ���
}

KVOID Kylin::PlayerController::FocusTarget( KUINT uTargetID )
{
	Kylin::Entity* pTarget = KylinRoot::GetSingletonPtr()->GetEntity(uTargetID);
	if (pTarget && pTarget != m_pHostChar)
	{
		// ʹ��ɫ���򽹵����
		KPoint3 kDir = pTarget->GetTranslate() - m_pHostChar->GetTranslate();		// B-A = A->B (see vector questions above)
		KPoint3 kSrc = m_pHostChar->GetRotation() * KPoint3::UNIT_Z;				// Orientation from initial direction
		kSrc.y = 0;																// Ignore pitch difference angle
		kDir.y = 0;
		kSrc.normalise();                                                           
		{                                                                           
			Ogre::Quaternion kQuat = kSrc.getRotationTo(kDir);                        
			m_pHostChar->GetSceneNode()->rotate(kQuat);                                                    
		}
		
		// ��UI����ʾ���������Ϣ
		if (BtIsKindOf(NpcObject,pTarget))
		{
			MonsterInfoMenu* pMenu = GET_GUI_PTR(MonsterInfoMenu);
			pMenu->SetVisible(true);
			// set anima ... 
		}

		// �ƶ�������Ч����Ŀ��λ��
		m_pFocusEffect->MoveTo(pTarget->GetTranslate());
		m_pFocusEffect->SetVisible(true);

		m_uTargetID = uTargetID;
	}
}

KVOID Kylin::PlayerController::UpdateEffect( KFLOAT fElapsed )
{
	if (m_uTargetID != INVALID_ID && m_pFocusEffect->IsVisible())
	{
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_uTargetID);
		if (pEnt)
		{
			KFLOAT fDistance = pEnt->GetTranslate().squaredDistance(m_pHostChar->GetTranslate());
			if (fDistance < CLICK_DISTANCE * CLICK_DISTANCE)
			{
				m_pFocusEffect->MoveTo(pEnt->GetTranslate());
				return;
			}
		}
		
		//-----------------------------------------------------------------
		m_pFocusEffect->SetVisible(false);
		// set ui
		MonsterInfoMenu* pMenu = GET_GUI_PTR(MonsterInfoMenu);
		pMenu->SetVisible(false);
		m_uTargetID = INVALID_ID;
	}
}

KBOOL Kylin::PlayerController::SelectedEntity( Ogre::Ray kRay )
{
	Ogre::Entity* pEnt = NULL;
	KPoint3 kHitPos; 
	if ( OgreUtils::PickEntity(kRay,&pEnt,kHitPos,KylinRoot::KR_NPC_MASK,CLICK_DISTANCE) )
	{
		if (pEnt)
		{
			KUINT uID = Ogre::any_cast<KUINT>(pEnt->getUserAny());
			Kylin::Entity* pTarget = KylinRoot::GetSingletonPtr()->GetEntity(uID);
			if (pTarget)
			{
				// ���ý�����ѡ�ж�����
				FocusTarget(uID);

				// ����Ĭ�ϼ���
				if (m_kSelectAction.uActionGID == INVALID_ID)
				{
					SelectDefaultAction();
				}

				if (m_kSelectAction.uActionType == AT_TAR)
				{	// ����������������ӽ��󹥻�
					KFLOAT fDistance = pTarget->GetTranslate().distance(m_pHostChar->GetTranslate());
					KFLOAT fValidDis = pTarget->GetBoundingRadius() + m_pHostChar->GetBoundingRadius();
					Action* pAct	= m_pHostChar->GetActionDispatcher()->GetActionPtr(m_kSelectAction.uActionGID);
					assert(pAct);
					if (pAct && pAct->GetRange() > 0)
						fValidDis = pAct->GetRange() + pTarget->GetBoundingRadius();

					if (fDistance > fValidDis)			
					{
						KPoint3 kDir = pTarget->GetTranslate() - m_pHostChar->GetTranslate();
						kDir.normalise();
						
						m_kMousePickPos = pTarget->GetTranslate() - kDir * fValidDis;
						m_fDistance		= fDistance - fValidDis;
					}
					else
					{
						SAFE_CALL(m_pHostChar->GetActionDispatcher(),Fire(m_kSelectAction.uActionGID,uID));
						m_kSelectAction.Reset();
					}
				}
				else if (m_kSelectAction.uActionType == AT_IMM)
				{
					SAFE_CALL(m_pHostChar->GetActionDispatcher(),Fire(m_kSelectAction.uActionGID));
					m_kSelectAction.Reset();
				}
			}
		}

		return true;
	}

	return false;
}

KVOID Kylin::PlayerController::SelectedTerrain( Ogre::Ray kRay )
{
	KPoint3 kHitPos; 
	//-------------------------------------------------------
	if (KylinRoot::GetSingletonPtr()->HitTest(kRay,kHitPos))
	{
		KFLOAT fDistance = (kHitPos - m_pHostChar->GetTranslate()).length();
		// �����ɵ������
		if ( fDistance < CLICK_DISTANCE )
		{
				//////////////////////////////////////////////////////////////////////////
		// ��ת��ʰȡ�ķ���
		KPoint3 kDir = kHitPos - m_pHostChar->GetTranslate();		// B-A = A->B (see vector questions above)
		KPoint3 kSrc = m_pHostChar->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
		kSrc.y = 0;														// Ignore pitch difference angle
		kDir.y = 0;
		kSrc.normalise();                                                           
		{                                                                           
			Ogre::Quaternion kQuat = kSrc.getRotationTo(kDir);                        
			m_pHostChar->GetSceneNode()->rotate(kQuat);                                                    
		}
			
			//-------------------------------------------------------
			// ʹ�ü���
			if (m_kSelectAction.uActionType == AT_POS)
			{
				SAFE_CALL(m_pHostChar->GetActionDispatcher(),Fire(m_kSelectAction.uActionGID,kHitPos));
				m_kSelectAction.Reset();
				return ;
			}
			
			CancelCurrentAction();
			//-------------------------------------------------------

			m_fDistance		= fDistance;
			m_kMousePickPos = kHitPos;
			
			// ��ѡ��λ�ò��Ŷ���
			m_pGuideEffect->MoveTo(kHitPos);
			m_pGuideEffect->SetVisible(true);

			// ���Ž�ɫ����
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHostChar,"do_walk");
		}
	}
}

KVOID Kylin::PlayerController::SelectDefaultAction()
{
	if (m_kSelectAction.uActionGID == INVALID_ID)
	{
		KANY aValue;
		if ( DataManager::GetSingletonPtr()->Select("ACTION_DB",m_uDefaultActionID,"TYPE",aValue) )
		{
			KSTR sValue = boost::any_cast<KSTR>(aValue);	

			ActionType type = Action::TransformType(sValue);
			
			m_kSelectAction.uActionGID  = m_uDefaultActionID;
			m_kSelectAction.uActionType = type;
		}
	}
}	

KVOID Kylin::PlayerController::SetDefaultAction( KUINT uActID )
{
	m_uDefaultActionID = uActID;
}

KVOID Kylin::PlayerController::CancelCurrentAction()
{
	if (m_kSelectAction.uActionGID != INVALID_ID)
	{
		m_kSelectAction.Reset();

		// �������

	}
}

Kylin::Character* Kylin::PlayerController::GetHostChar()
{
	return m_pHostChar;
}
