#pragma once

#include "action.h"

// ���䡢�ָ��Ի���
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