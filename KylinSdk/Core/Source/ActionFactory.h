#pragma once

namespace Kylin
{
	class ActionFactory
	{
	public:
		ActionFactory(ActionDispatcher* pDispatcher) : m_pDispatcher(pDispatcher) {}
		virtual Action* Generate(KUINT uGID) = 0;
	protected:
		ActionDispatcher* m_pDispatcher;
	};
}