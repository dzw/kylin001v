#pragma once

#include "factor.h"
#include "ClockingCallback.h"


namespace Kylin
{
	class BulletFactor : public Factor 
					   , public ClockingCallback
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(BulletFactor, Factor)
		Declare_Entity_Creator(BulletFactor)

		BulletFactor();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID BindEffect(PropertySet kProp);
		virtual KVOID SetupCllsn(PropertySet kProp);

		virtual KVOID Moving(KFLOAT fElapsed);
	
	protected:
		virtual KVOID EV_PostTouchdown(EventPtr spEV);

	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
		virtual KVOID EndTime(KCSTR& sClass,KCSTR& sName, KANY aUserData);

	protected:
		KFLOAT m_fVelocity;		// ËÙ¶È
		
	};
}