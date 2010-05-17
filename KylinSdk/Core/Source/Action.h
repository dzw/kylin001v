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
		
		virtual Factor* SpawnFactor(KUINT uHostID) = 0;
		
		virtual KUINT GetGID();

	protected:
		virtual KVOID OnTriggered(Factor* pFactor);
		
	protected:
		friend class ActionDispatcher;
		friend class Factor;

		PropertySet m_kProperty;

// 		KUINT m_uGID;
// 		KUINT m_uMinFactorCount;

		typedef KLIST<KUINT> FactorList;
		FactorList m_kFactorList;

		ActionDispatcher* m_pDispatcher;
	};
}


