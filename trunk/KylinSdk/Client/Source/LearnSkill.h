#pragma once

#include "action.h"

// ѧϰ�ͼ���
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