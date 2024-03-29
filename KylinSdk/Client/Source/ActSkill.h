#pragma once

#include "action.h"

namespace Kylin
{
	class ActSkill : public Action
	{
	public:
		ActSkill(ActionDispatcher* pDispatcher);

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);

	protected:
		virtual KVOID HitTarget(KUINT uID);
	};
}