#pragma once

#include "factor.h"
#include "ClockingCallback.h"


namespace Kylin
{
	class CollapsarFactor : public Factor
						  , public ClockingCallback
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(CollapsarFactor, Factor)
		Declare_Entity_Creator(CollapsarFactor)

		CollapsarFactor();

	protected:
		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);

		virtual KVOID BindEffect(PropertySet kProp);

		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
		virtual KVOID EndTime(KCSTR& sClass,KCSTR& sName, KANY aUserData);

	protected:
		KVEC<KUINT>		m_kCaptives;
	};
}