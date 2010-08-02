#include "corepch.h"
#include "AI.h"
#include "Character.h"

#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "KylinRoot.h"
#include "randomc.h"
#include "rOgreUtils.h"
#include "Action.h"
#include "Factor.h"
#include "ActionDispatcher.h"
#include "Pathway.h"


#define _MAX_STAY_T_ 5
#define _MIN_STAY_T_ 1

Kylin::BaseAI::BaseAI( Character* pHost )
: m_pHostChar(pHost)
, m_pPathway(NULL)
, m_pRandomGenerator(NULL)
, m_eCurrState(AS_INVALID)
, m_uTargetFoe(INVALID_ID)
, m_fDistance(.0f)
, m_kDestination(KPoint3::ZERO)
, m_nPathwayIndex(0)
, m_fScanTime(.0f)
, m_bToBlock(false)
, m_fSpeed(1.0f)
{

}

Kylin::BaseAI::~BaseAI()
{
	SAFE_DEL(m_pRandomGenerator);
}

KBOOL Kylin::BaseAI::Init()
{
	m_pRandomGenerator = KNEW CRandomMersenne(this->m_pHostChar->GetID());

	m_fStayTime		= m_pRandomGenerator->IRandom(_MIN_STAY_T_,_MAX_STAY_T_);
	m_eCurrState	= AS_IDLE;
	
	//---------------------------------------------------------------
	
	return true;
}

KVOID Kylin::BaseAI::Tick( KFLOAT fElapsed )
{
	KBOOL bRet = true;
	switch(m_eCurrState)
	{
	case AS_IDLE:
		
		bRet = false;
		if (!m_pPathway)
			bRet = Tick_Radar(fElapsed);

		if (!bRet)
			bRet = Tick_Idle(fElapsed);
		break;

	case AS_MOVE:
		
		bRet = Tick_Radar(fElapsed);
		if (!bRet)
			bRet = Tick_Move(fElapsed);
		break;

	case AS_USE_SKILL:

		Tick_UseSkill(fElapsed);
// 		if (!bRet)
// 			bRet = Tick_Move(fElapsed);

		break;

	case AS_JUMP:

		bRet = Tick_Jump(fElapsed);
		break;

	case AS_DEAD:

		bRet = Tick_Dead(fElapsed);
		break;

	case AS_FOLLOW:

		bRet = Tick_Follow(fElapsed);
		break;
	
	case AS_PURSUE:
		
		bRet = Tick_Pursue(fElapsed);
		break;

	//default:
	}

	//---------------------------------------------------------------
	if (!bRet)
	{
		m_eCurrState = AS_INVALID;
	}
}

KVOID Kylin::BaseAI::Reset()
{

}

RC_RESULT Kylin::BaseAI::Enter_Idle( KVOID )
{
	if ( m_eCurrState != AS_IDLE )
	{
		// play char idle animation
		KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHostChar,"do_idle");
	}

	//---------------------------------------------------------------
	m_eCurrState = AS_IDLE;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_Move( FLOAT fDestX, FLOAT fDestZ )
{
	// 播放角色动画
	KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHostChar,"do_walk");
	//---------------------------------------------------------------
	m_kDestination.x = fDestX;
	m_kDestination.z = fDestZ;
	KPoint3 kDir = m_kDestination - m_pHostChar->GetTranslate();		// B-A = A->B (see vector questions above)
	KPoint3 kSrc = m_pHostChar->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
	kSrc.y = 0;														// Ignore pitch difference angle
	kDir.y = 0;
	kSrc.normalise(); 

	Ogre::Quaternion kQuat = kSrc.getRotationTo(kDir);
	m_pHostChar->GetSceneNode()->rotate(kQuat);

	m_fDistance = (m_kDestination - m_pHostChar->GetTranslate()).length();
	//---------------------------------------------------------------
	m_eCurrState = AS_MOVE;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KPoint3 kDir )
{
	// 在释放技能之前先面向敌人
	KPoint3 kSrc = m_pHostChar->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
	kSrc.y = 0;														// Ignore pitch difference angle
	kSrc.normalise(); 

	Ogre::Quaternion kQuat = kSrc.getRotationTo(kDir);
	m_pHostChar->GetSceneNode()->rotate(kQuat);
	
	Factor* pFactor = m_pHostChar->GetActionDispatcher()->Fire(uSkillId,uTarget);
	//---------------------------------------------------------------
	Action* pAction = m_pHostChar->GetActionDispatcher()->GetActionPtr(uSkillId);
	KFLOAT fTimes = 0.6f;
	if (pAction->GetPropertyRef().GetFloatValue("$AnimLength",fTimes))
		fTimes *= 1.2f;
	m_fStayTime = fTimes;

	m_uTargetFoe = uTarget;

	m_eCurrState = AS_USE_SKILL;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_Dead( KVOID )
{

	//---------------------------------------------------------------
	m_eCurrState = AS_DEAD;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_Follow( KUINT uTargetObj )
{

	//---------------------------------------------------------------
	m_eCurrState = AS_FOLLOW;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_Jump( KVOID )
{
	//---------------------------------------------------------------
	m_eCurrState = AS_JUMP;
	return RC_OK;
}

KBOOL Kylin::BaseAI::Tick_Idle( KFLOAT fElapsed )
{
	if (m_fStayTime > 0 && m_pPathway)
	{
		m_fStayTime-=fElapsed;
		if (m_fStayTime < 0)
		{
			Enter_Patrol();
			m_fStayTime = m_pRandomGenerator->Random();
		}
	}

	return true;
}


KBOOL Kylin::BaseAI::Tick_Move( KFLOAT fElapsed )
{
	KFLOAT fOffset = fElapsed * m_fSpeed;
	m_fDistance -= fOffset;
	if (m_fDistance < KZERO)
	{				
		m_kDestination = KPoint3::ZERO;
		
		// 切换状态
		Enter_Idle();
		//-----------------------------------------------------------
		if (m_nPathwayIndex == m_pPathway->Size())
		{
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(m_pHostChar,"on_endpoint");
		}
	}
	else 
	{
		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(0, 0, m_fSpeed));
	}

	return true;
}

KBOOL Kylin::BaseAI::Tick_UseSkill( KFLOAT fElapsed )
{
	if (m_fStayTime > .0f)
	{
		m_fStayTime -= fElapsed;
		if (m_fStayTime <= .0f)
		{
			Action* pAction = m_pHostChar->GetActionDispatcher()->GetFirstActionPtr();
			if (pAction)
			{	// 若该NPC存在技能
				KFLOAT fRange = m_pHostChar->GetEntityPtr()->getBoundingRadius() + pAction->GetRange();

				Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_uTargetFoe);
				if ( pEnt && pEnt->GetTranslate().squaredDistance(m_pHostChar->GetTranslate()) < fRange * fRange )
				{
					Character* pTar = BtDynamicCast(Character,pEnt);

					if ( pTar && 
						KylinRoot::GetSingletonPtr()->CheckRelation(pTar,m_pHostChar) == KylinRoot::RELATION_ENEMY )
					{
						KPoint3 kDir = pEnt->GetTranslate() - m_pHostChar->GetTranslate();
						kDir.y = 0;

						Enter_UseSkill(pAction->GetGID(),m_uTargetFoe,pEnt->GetTranslate(),kDir);
						return true;
					}
				}
				
				// 当敌人死亡或超出攻击距离
				m_uTargetFoe = INVALID_ID;
			}
			
			m_fStayTime = m_pRandomGenerator->Random();
			if (m_pPathway)
				Enter_Move(m_kDestination.x,m_kDestination.z);
			else
				Enter_Idle();

			return true;
		}
	}

	return false;
}

KBOOL Kylin::BaseAI::Tick_Dead( KFLOAT fElapsed )
{
	return true;
}

KBOOL Kylin::BaseAI::Tick_Follow( KFLOAT fElapsed )
{
	return true;
}

// KBOOL Kylin::BaseAI::Tick_AutoHit( KFLOAT fElapsed )
// {
// 	return true;
// }

KBOOL Kylin::BaseAI::Tick_Jump( KFLOAT fElapsed )
{
	
	return true;
}

KVOID Kylin::BaseAI::SetPathway( Pathway* pPaths )
{
	m_pPathway = pPaths;
}

RC_RESULT Kylin::BaseAI::Enter_Patrol()
{
	RC_RESULT kRet = RC_ERROR;
	if (m_nPathwayIndex >= 0 && m_nPathwayIndex < m_pPathway->Size())
	{
		kRet = Enter_Move( m_pPathway->At(m_nPathwayIndex).x, m_pPathway->At(m_nPathwayIndex).z );
		m_nPathwayIndex++;
	}
	else if (m_pPathway->IsTurnback() && m_nPathwayIndex == m_pPathway->Size())
	{
		m_nPathwayIndex = 0;
	}

	return kRet;
}

// test code
const float	g_radar_time	= 0.3f;
const float g_adjust_angle	= 10.0f;
KBOOL Kylin::BaseAI::Tick_Radar( KFLOAT fElapsed )
{
	m_fScanTime += fElapsed;
	if (m_fScanTime > g_radar_time)
	{
		m_fScanTime = .0f;
		//-----------------------------------------------------------
		// radar
// 		KPoint3	kPos = m_pHostChar->GetTranslate();
// 		kPos.y += m_pHostChar->GetEntityPtr()->getBoundingRadius();
//		KPoint3	kDes = m_kDestination;
//		kDes.y = kPos.y;

//		Ogre::Ray kRay(kPos,kDes);
//		Ogre::Entity* pEnt = NULL;
		
// 		m_bToBlock = OgreUtils::PickEntityBoundBox(kRay,&pEnt,kPos,KylinRoot::KR_SCENE_OBJ,VISIBLE_DISTANCE); //  | KylinRoot::KR_NPC_MASK
// 		if (m_bToBlock)
// 		{
// 			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(m_fSpeed, 0, 0));
// 			// random move dir ( left or right )
// 			// radar
// 		}
// 		else
		{
			Action* pAction = m_pHostChar->GetActionDispatcher()->GetFirstActionPtr();
			if (pAction)
			{	// 若该NPC存在技能
				KFLOAT fRange = m_pHostChar->GetEntityPtr()->getBoundingRadius() + pAction->GetRange();

				KVEC<Ogre::Entity*> kEntities;
				Kylin::OgreUtils::SphereQuery(m_pHostChar->GetTranslate(),fRange,kEntities,Kylin::KylinRoot::KR_NPC_MASK);

				for (KUINT i = 0; i < kEntities.size(); i++)
				{
					if (!kEntities[i]->getUserAny().isEmpty())
					{
						KUINT uID = Ogre::any_cast<KUINT>(kEntities[i]->getUserAny());

						Character* pTar = BtDynamicCast(Character,KylinRoot::GetSingletonPtr()->GetEntity(uID));

						if ( pTar && 
							KylinRoot::GetSingletonPtr()->CheckRelation(pTar,m_pHostChar) == KylinRoot::RELATION_ENEMY )
						{
							//---------------------------------------
							KPoint3 kDir = pTar->GetTranslate() - m_pHostChar->GetTranslate();
							kDir.y = 0;

							Enter_UseSkill(pAction->GetGID(),uID,pTar->GetTranslate(),kDir);

							return true;
						}
					}
				}
			}
		}
	}
		
	return false;
}

RC_RESULT Kylin::BaseAI::Enter_Pursue( KUINT uTargetObj )
{
	m_uTargetFoe = uTargetObj;

	//---------------------------------------------------------------
	m_eCurrState = AS_PURSUE;
	return RC_OK;
}

KBOOL Kylin::BaseAI::Tick_Pursue( KFLOAT fElapsed )
{
	

	return true;
}