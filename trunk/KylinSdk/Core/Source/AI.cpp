#include "corepch.h"
#include "AI.h"
#include "Character.h"

#include "rPhyXSystem.h"
#include "rMotionSimulator.h"

#include "randomc.h"

#define _MAX_STAY_T_ 5
#define _MIN_STAY_T_ 1

Kylin::BaseAI::BaseAI( Character* pHost )
: m_pHostChar(NULL)
, m_pRandomGenerator(NULL)
, m_eCurrState(AS_INVALID)
, m_fDistance(.0f)
, m_kDestination(KPoint3::ZERO)
, m_nPathwayIndex(0)
{

}

Kylin::BaseAI::~BaseAI()
{
	SAFE_DEL(m_pRandomGenerator);
}

KBOOL Kylin::BaseAI::Init()
{
	m_pRandomGenerator = KNEW CRandomMersenne(this->m_pHostChar->GetID());

	m_fStayTime = m_pRandomGenerator->IRandom(_MIN_STAY_T_,_MAX_STAY_T_);

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

	}

	//---------------------------------------------------------------
	m_eCurrState = AS_IDLE;
	return RC_OK;
}

RC_RESULT Kylin::BaseAI::Enter_Move( FLOAT fDestX, FLOAT fDestZ )
{
	m_kDestination.x = fDestX;
	m_kDestination.z = fDestZ;
	KPoint3 kDir = m_kDestination - m_pHostChar->GetTranslate();		// B-A = A->B (see vector questions above)
	KPoint3 kSrc = m_pHostChar->GetRotation() * KPoint3::UNIT_Z;		// Orientation from initial direction
	kSrc.y = 0;														// Ignore pitch difference angle
	kDir.y = 0;
	kSrc.normalise(); 

	Quaternion kQuat = kSrc.getRotationTo(kDir);
	m_pHostChar->GetSceneNode()->rotate(kQuat);

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
			//Enter_Move();
			//m_fStayTime = m_pRandomGenerator->Random();
		}
	}

	return true;
}

// test code
const int g_speed = 9;
KBOOL Kylin::BaseAI::Tick_Move( KFLOAT fElapsed )
{
	KPoint3 kDir = m_kDestination - m_pHostChar->GetTranslate();
	kDir.normalise();
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


