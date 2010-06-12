#pragma once

#include "factor.h"

namespace Kylin
{
	class BulletFactor : public Factor
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(BulletFactor, Factor)
		Declare_Entity_Creator(BulletFactor)

		BulletFactor();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		

	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
	
	protected:

	};
}