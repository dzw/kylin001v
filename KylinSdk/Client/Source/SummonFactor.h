#pragma once

#include "factor.h"
#include "ClockingCallback.h"

//
// ’ŸªΩ–Õ“Ú◊”
//
namespace Kylin
{
	class SummonFactor : public Factor 
					   , public ClockingCallback
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(SummonFactor, Factor)
		Declare_Entity_Creator(SummonFactor)

		SummonFactor();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID BindEffect(PropertySet kProp);

		virtual KVOID Moving(KFLOAT fElapsed);
	
	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
		virtual KVOID EndTime(KCSTR& sClass,KCSTR& sName, KANY aUserData);
		
		virtual KVOID DoSummon();

	protected:
		KFLOAT	m_fVelocity;
	};
}