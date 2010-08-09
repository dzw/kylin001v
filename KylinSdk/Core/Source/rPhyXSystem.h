#pragma once

#include "singleton.h"

namespace Kylin
{
	namespace PhyX
	{	
		class CoverMonitor;
		class CollisionMonitor;
		class MotionSimulator;
		class PhysicalSystem : public Singleton<PhysicalSystem>
		{
		public:
			PhysicalSystem();
			~PhysicalSystem();

			KVOID CreateMotionSimulator(MotionSimulator* pMs = NULL);
			KVOID CreateCollisionMonitor(CollisionMonitor* pCllsn = NULL);
			KVOID CreateCoverMonitor(CoverMonitor* pCm = NULL);

			KVOID Tick(KFLOAT fElapsed);
			
			MotionSimulator*	GetMotionSimulator();
			CollisionMonitor*	GetCollisionMonitor();
			CoverMonitor*		GetCoverMonitor();
		
			KVOID				SetEnable(KBOOL bFlag) { m_bEnable = bFlag; }
			KBOOL				IsEnable() { return m_bEnable; }
			KVOID				Clear();

		protected:
			CoverMonitor*		m_pCoverMonitor;
			MotionSimulator*	m_pMotionSimulator;
			CollisionMonitor*	m_pCollisionMonitor;

			KBOOL				m_bEnable;
		};
	}
}