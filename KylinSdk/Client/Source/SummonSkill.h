#pragma once

#include "ActSkill.h"

namespace Kylin
{
	class BeckonSkill : public ActSkill
	{
	public:
		BeckonSkill(ActionDispatcher* pDispatcher);

		virtual KBOOL Init(const PropertySet& kProp);

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual Factor* SpawnFactor();

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);

	protected:

	};
}