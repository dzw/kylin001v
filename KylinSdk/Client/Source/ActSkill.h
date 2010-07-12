#pragma once

#include "action.h"

namespace Kylin
{
	class ActSkill : public Action
	{
	public:
		ActSkill(ActionDispatcher* pDispatcher);

		virtual KBOOL Init(const PropertySet& kProp);

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual Factor* SpawnFactor();

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);
		virtual KVOID RevertHost();

	protected:
		virtual KVOID HitTarget(KUINT uID);
	};
}