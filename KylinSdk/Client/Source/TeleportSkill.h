#pragma once

#include "ActSkill.h"

namespace Kylin
{
	class TeleportSkill : public ActSkill
	{
	public:
		TeleportSkill(ActionDispatcher* pDispatcher);

		virtual KBOOL Init(const PropertySet& kProp);

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual Factor* SpawnFactor();

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);

	protected:

	};
}