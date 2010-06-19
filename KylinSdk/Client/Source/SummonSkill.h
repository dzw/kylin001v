#pragma once

#include "action.h"


namespace Kylin
{
	class SummonSkill : public Action
	{
	public:

		SummonSkill(ActionDispatcher* pDispatcher);

		virtual KBOOL Init(const PropertySet& kProp);

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual Factor* SpawnFactor();
	
	protected:
		virtual KVOID OnTriggered(Factor* pFactor);

	protected:
		
	};
}