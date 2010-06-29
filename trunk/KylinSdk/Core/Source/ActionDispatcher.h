#pragma once

namespace Kylin
{
	class ActionDispatcher
	{
	public:
		ActionDispatcher(KUINT uHostID);
		
		virtual Factor* Fire(KUINT uGID/*,const KPoint3& kPos*/);
		virtual Factor* Fire(KUINT uGID,const KPoint3& kPos);
		virtual Factor* Fire(KUINT uGID,KUINT uTarget);

		virtual KVOID	SetFactory(ActionFactory* pFactory);
		virtual Action* SpawnAction(KUINT uGID);

		virtual KVOID	DestroyAction(Action* pAct);
		virtual KVOID	DestroyAllAction();

		virtual KVOID	Tick(KFLOAT fElapsed);

	public:
		KVOID SetHost(Entity* pEnt);
		KUINT GetHostWorldID() { return m_uHostID; }

	protected:
		Action* GetActionPtr(KUINT uGID);

	protected:
		typedef KLIST<Action*> ActionList;
		ActionList		m_kActionList;

		ActionFactory*	m_pActionFactory;

		// host id
		KUINT			m_uHostID;
		
		// �Ƿ�æ
		KBOOL			m_bBusy;
	};
}

