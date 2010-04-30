#pragma once

namespace Kylin
{
	class Action
	{
	public:
		Action(Entity* pHost);
		virtual ~Action();
		
		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
	public:
		virtual KVOID AddTarget(KUINT uTargetID);
		virtual KVOID RemoveTarget(KUINT uTargetID);

	protected:
		KVEC<KUINT>		m_kTargets;
		Entity*			m_pHost;


	};
}