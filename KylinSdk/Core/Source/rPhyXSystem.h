#pragma once

#include "singleton.h"

namespace Kylin
{
	namespace PhyX
	{	
		class MotionSimulator;
		class PhysicalSystem : public Singleton<PhysicalSystem>
		{
		public:
			PhysicalSystem();
			~PhysicalSystem();

			KVOID CreateMotionSimulator(MotionSimulator* pMs = NULL);
			KVOID Tick(KFLOAT fElapsed);
			
			MotionSimulator* GetMotionSimulator();

		protected:
			MotionSimulator* m_pMotionSimulator;
		};
	}
}