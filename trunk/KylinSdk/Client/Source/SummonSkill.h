#pragma once

#include "ActSkill.h"

//
// �ٻ��ͼ���
//
namespace Kylin
{
	class SummonSkill : public ActSkill
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