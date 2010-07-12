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
		
	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
	
	};
}