#include "corepch.h"
#include "rPhyXSystem.h"
#include "rMotionSimulator.h"


Kylin::PhyX::PhysicalSystem::PhysicalSystem()
: m_pMotionSimulator(NULL)
{

}

Kylin::PhyX::PhysicalSystem::~PhysicalSystem()
{
	SAFE_CALL(m_pMotionSimulator,Destroy());
	SAFE_DEL(m_pMotionSimulator);
}

KVOID Kylin::PhyX::PhysicalSystem::CreateMotionSimulator( MotionSimulator* pMs /*= NULL*/ )
{
	if (!pMs)
		m_pMotionSimulator = KNEW MotionSimulator();
	else
		m_pMotionSimulator = pMs;
}

KVOID Kylin::PhyX::PhysicalSystem::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pMotionSimulator,Tick(fElapsed));
}

Kylin::PhyX::MotionSimulator* Kylin::PhyX::PhysicalSystem::GetMotionSimulator()
{
	return m_pMotionSimulator;
}