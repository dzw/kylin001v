#pragma once

#include "factor.h"


namespace Kylin
{
	class ChainFactor : public Factor 
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(ChainFactor, Factor)
		Declare_Entity_Creator(ChainFactor)

		ChainFactor();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		
		virtual KVOID SetTarget(KUINT uTarget);

	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
	
	protected:
		KUINT	m_uTarget;
	};
}