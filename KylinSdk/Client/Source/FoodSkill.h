#pragma once

#include "action.h"

namespace Kylin
{
	class FoodSkill : public Action
	{
	public:
		FoodSkill(ActionDispatcher* pDispatcher);

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);

	};
}