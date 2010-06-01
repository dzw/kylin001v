#pragma once

#include "singleton.h"

namespace Kylin
{
	namespace PhyX
	{	
		class CollisionMonitor;
		class MotionSimulator;
		class PhysicalSystem : public Singleton<PhysicalSystem>
		{
		public:
			PhysicalSystem();
			~PhysicalSystem();

			KVOID CreateMotionSimulator(MotionSimulator* pMs = NULL);
			KVOID CreateCollisionMonitor(CollisionMonitor* pCllsn = NULL);

			KVOID Tick(KFLOAT fElapsed);
			
			MotionSimulator*	GetMotionSimulator();
			CollisionMonitor*	GetCollisionMonitor();

		protected:
			MotionSimulator*	m_pMotionSimulator;
			CollisionMonitor*	m_pCollisionMonitor;
		};
	}
}