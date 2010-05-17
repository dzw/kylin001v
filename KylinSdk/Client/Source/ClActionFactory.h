#pragma once

#include "ActionFactory.h"

namespace Kylin
{
	class ClActionFactory : public ActionFactory
	{
	public:
		ClActionFactory(ActionDispatcher* pDispatcher) : ActionFactory(pDispatcher) {}

		virtual Action* Generate(KUINT uGID);
	};
}