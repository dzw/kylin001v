#pragma once

namespace Kylin
{
	class ActionDispatcher
	{
	public:
		ActionDispatcher(KUINT uHostID);
		
		virtual KVOID	SetFactory(ActionFactory* pFactory);
		virtual Action* SpawnAction(KUINT uGID,const KPoint3& kPos);

		virtual KVOID	DestroyAction(Action* pAct);
		virtual KVOID	DestroyAllAction();

		virtual KVOID	Tick(KFLOAT fElapsed);

	public:
		KVOID SetHost(Entity* pEnt);

	protected:
		Action* GetActionPtr(KUINT uGID);

	protected:
		typedef KLIST<Action*> ActionList;
		ActionList		m_kActionList;

		ActionFactory*	m_pActionFactory;
		KUINT			m_uHostID;
	};
}

