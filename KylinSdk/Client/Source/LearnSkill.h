#pragma once

#include "action.h"

// 学习型技能
namespace Kylin
{
	class LearnSkill : public Action
	{
	public:
		LearnSkill(ActionDispatcher* pDispatcher);

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);
	};
}