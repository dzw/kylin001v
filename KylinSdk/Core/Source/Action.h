#pragma once

#include "Property.h"

namespace Kylin
{
	class Action
	{
	public:
		Action(ActionDispatcher* pDispatcher);
		virtual ~Action();

		virtual KBOOL Init(const PropertySet& kProp);

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KBOOL IsComplete();
		
		virtual Factor* SpawnFactor();
		virtual KVOID	RemoveFactor(KUINT uFactorID);

		virtual KUINT GetGID();

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);
		
	protected:
		friend class ActionDispatcher;
		friend class Factor;

		PropertySet m_kProperty;

		typedef KLIST<KUINT> FactorList;
		FactorList m_kFactorList;

		ActionDispatcher* m_pDispatcher;
	};
}


