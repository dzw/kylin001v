#include "corepch.h"
#include "AI.h"
#include "Character.h"

#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "KylinRoot.h"
#include "randomc.h"
#include "rOgreUtils.h"

#define _MAX_STAY_T_ 5
#define _MIN_STAY_T_ 1

Kylin::BaseAI::BaseAI( Character* pHost )
: m_pHostChar(pHost)
, m_pRandomGenerator(NULL)
, m_eCurrState(AS_INVALID)
, m_fDistance(.0f)
, m_kDestination(KPoint3::ZERO)
, m_nPathwayIndex(0)
, m_fScanTime(.0f)
, m_bToBlock(false)
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

	return true;
}

KVOID Kylin::BaseAI::Tick( KFLOAT fElapsed )
{
	KBOOL bRet = true;
	switch(m_eCurrState)
	{
	case AS_IDLE:
		
		bRet = Tick_Idle(fElapsed);
		break;

	case AS_MOVE:
		
		bRet = false;//Tick_Radar(fElapsed);
		if (!bRet)
			bRet = Tick_Move(fElapsed);
		break;

	case AS_USE_SKILL:

		bRet = Tick_UseSkill(fElapsed);
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
	// ²¥·Å½ÇÉ«¶¯»­
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

RC_RESULT Kylin::BaseAI::Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KFLOAT fDir )
{

	//---------------------------------------------------------------
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
	if (m_fStayTime > 0)
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

// test code
const int g_speed = 5;
KBOOL Kylin::BaseAI::Tick_Move( KFLOAT fElapsed )
{
	KFLOAT fOffset = fElapsed * g_speed;
	m_fDistance -= fOffset;
	if (m_fDistance < KZERO)
	{				
		m_kDestination = KPoint3::ZERO;

		// ÇÐ»»×´Ì¬
		Enter_Idle();
	}
	else 
	{
		// move in current body direction (not the goal direction)
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(0, 0, g_speed));
	}

	return true;
}

KBOOL Kylin::BaseAI::Tick_UseSkill( KFLOAT fElapsed )
{
	return true;
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

KVOID Kylin::BaseAI::SetPathway( const Pathway& kPaths )
{
	m_kPathway = kPaths;
}

KVOID Kylin::BaseAI::AddPathwayPos( const KPoint3& kPos )
{
	m_kPathway.push_back(kPos);
}

RC_RESULT Kylin::BaseAI::Enter_Patrol()
{
	RC_RESULT kRet = RC_RESULT::RC_ERROR;
	if (m_nPathwayIndex >= 0 && m_nPathwayIndex < m_kPathway.size())
	{
		kRet = Enter_Move(m_kPathway[m_nPathwayIndex].x,m_kPathway[m_nPathwayIndex].z);
		m_nPathwayIndex++;
		if (m_nPathwayIndex == m_kPathway.size())
		{
			m_nPathwayIndex = 0;
		}
	}
		
	return kRet;
}

// test code
const int	g_radar_time	= 0.3f;
const float g_adjust_angle	= 10.0f;
KBOOL Kylin::BaseAI::Tick_Radar( KFLOAT fElapsed )
{
	m_fScanTime += fElapsed;
	if (m_fScanTime > g_radar_time)
	{
		//--------------------
		// radar
		KPoint3	kPos = m_pHostChar->GetTranslate();
		kPos.y += m_pHostChar->GetEntityPtr()->getBoundingRadius();
		KPoint3	kDes = m_kDestination;
		kDes.y = kPos.y;

		Ogre::Ray kRay(kPos,kDes);
		Ogre::Entity* pEnt = NULL;
		
		m_bToBlock = OgreUtils::PickEntityBoundBox(kRay,&pEnt,kPos,KylinRoot::KR_SCENE_OBJ | KylinRoot::KR_NPC_MASK,g_adjust_angle);

		if (m_bToBlock)
		{
			Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(g_speed, 0, 0));
			// random move dir ( left or right )
			// radar
		}
			
		m_fScanTime = .0f;
	}

	if (m_bToBlock)
	{
		Kylin::PhyX::PhysicalSystem::GetSingletonPtr()->GetMotionSimulator()->Commit(m_pHostChar,KPoint3(g_speed, 0, 0));
	}
		
	return m_bToBlock;
}
