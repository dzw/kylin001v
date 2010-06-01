#include "corepch.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"
#include "rCollisionMonitor.h"


Kylin::PhyX::PhysicalSystem::PhysicalSystem()
: m_pMotionSimulator(NULL)
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

KVOID Kylin::PhyX::PhysicalSystem::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pMotionSimulator,Tick(fElapsed));
	SAFE_CALL(m_pCollisionMonitor,Tick(fElapsed));
}

Kylin::PhyX::MotionSimulator* Kylin::PhyX::PhysicalSystem::GetMotionSimulator()
{
	return m_pMotionSimulator;
}

Kylin::PhyX::CollisionMonitor* Kylin::PhyX::PhysicalSystem::GetCollisionMonitor()
{
	return m_pCollisionMonitor;
}
