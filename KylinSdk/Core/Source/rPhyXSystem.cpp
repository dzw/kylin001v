#include "corepch.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "rCollisionMonitor.h"
#include "rCoverMonitor.h"


Kylin::PhyX::PhysicalSystem::PhysicalSystem()
: m_pCoverMonitor(NULL)
, m_pMotionSimulator(NULL)
, m_pCollisionMonitor(NULL)
{

}

Kylin::PhyX::PhysicalSystem::~PhysicalSystem()
{
	SAFE_CALL(m_pMotionSimulator,Destroy());
	SAFE_DEL(m_pMotionSimulator);
	//-------------------------------------------------------------------------
	SAFE_CALL(m_pCollisionMonitor,Destroy());
	SAFE_DEL(m_pCollisionMonitor);
	//-------------------------------------------------------------------------
	SAFE_CALL(m_pCoverMonitor,Destroy());
	SAFE_DEL(m_pCoverMonitor);
}

KVOID Kylin::PhyX::PhysicalSystem::CreateMotionSimulator( MotionSimulator* pMs /*= NULL*/ )
{
	if (!pMs)
		m_pMotionSimulator = KNEW MotionSimulator();
	else
		m_pMotionSimulator = pMs;
}


KVOID Kylin::PhyX::PhysicalSystem::CreateCollisionMonitor( CollisionMonitor* pCllsn /*= NULL*/ )
{
	if (!pCllsn)
		m_pCollisionMonitor = KNEW CollisionMonitor();
	else
		m_pCollisionMonitor = pCllsn;
}

KVOID Kylin::PhyX::PhysicalSystem::CreateCoverMonitor( CoverMonitor* pCm /*= NULL*/ )
{
	if (!pCm)
		m_pCoverMonitor = KNEW CoverMonitor();
	else
		m_pCoverMonitor = pCm;
}

KVOID Kylin::PhyX::PhysicalSystem::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pMotionSimulator,Tick(fElapsed));
	SAFE_CALL(m_pCollisionMonitor,Tick(fElapsed));
	SAFE_CALL(m_pCoverMonitor,Tick(fElapsed));
}

Kylin::PhyX::MotionSimulator* Kylin::PhyX::PhysicalSystem::GetMotionSimulator()
{
	return m_pMotionSimulator;
}

Kylin::PhyX::CollisionMonitor* Kylin::PhyX::PhysicalSystem::GetCollisionMonitor()
{
	return m_pCollisionMonitor;
}

Kylin::PhyX::CoverMonitor* Kylin::PhyX::PhysicalSystem::GetCoverMonitor()
{
	return m_pCoverMonitor;
}
