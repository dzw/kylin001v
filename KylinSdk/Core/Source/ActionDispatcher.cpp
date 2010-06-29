#include "corepch.h"
#include "ActionDispatcher.h"
#include "Action.h"
#include "Factor.h"
#include "ActionFactory.h"
#include "KylinRoot.h"


Kylin::ActionDispatcher::ActionDispatcher( KUINT uHostID )
: m_uHostID(uHostID)
, m_pActionFactory(NULL)
, m_bBusy(false)
{

}


Kylin::Action* Kylin::ActionDispatcher::SpawnAction( KUINT uGID)
{
	assert(m_pActionFactory);
	Action* pAct = GetActionPtr(uGID);
	if (!pAct)
	{
		pAct = m_pActionFactory->Generate(uGID);
		m_kActionList.push_back(pAct);
	}
	
	return pAct;
}

KVOID Kylin::ActionDispatcher::DestroyAction( Kylin::Action* pAction)
{
	for ( ActionList::iterator it = m_kActionList.begin(); 
		it != m_kActionList.end(); 
		it++ 
		)
	{
		if ((*it) == pAction)
		{
			pAction->Destroy();
			SAFE_DEL(pAction);
			m_kActionList.erase(it);
			break;
		}
	}
}

KVOID Kylin::ActionDispatcher::Tick( KFLOAT fElapsed )
{
	for ( ActionList::iterator it = m_kActionList.begin(); 
		  it != m_kActionList.end(); 
		  it++ 
		 )
	{
		(*it)->Tick(fElapsed);
	}
}

KVOID Kylin::ActionDispatcher::DestroyAllAction()
{
	for ( ActionList::iterator it = m_kActionList.begin(); 
		it != m_kActionList.end(); 
		it++ 
		)
	{
		(*it)->Destroy();
		SAFE_DEL(*it);
	}
	m_kActionList.clear();
	//////////////////////////////////////////////////////////////////////////
	SAFE_DEL(m_pActionFactory);
}

Kylin::Action* Kylin::ActionDispatcher::GetActionPtr( KUINT uGID )
{
	for ( ActionList::iterator it = m_kActionList.begin(); 
		it != m_kActionList.end(); 
		it++ 
		)
	{
		if ((*it)->GetGID() == uGID)
		{
			return *it;
		}
	}

	return NULL;
}

KVOID Kylin::ActionDispatcher::SetHost( Kylin::Entity* pEnt )
{
	m_uHostID = pEnt->GetID();
}

KVOID Kylin::ActionDispatcher::SetFactory( ActionFactory* pFactory )
{
	assert(pFactory);
	m_pActionFactory = pFactory;
}

Kylin::Factor* Kylin::ActionDispatcher::Fire( KUINT uGID/*,const KPoint3& kPos*/ )
{
	Factor* pFactor = NULL;
	Action* pAct	= GetActionPtr(uGID);
	if (pAct)
	{
		pFactor = pAct->SpawnFactor();
		//SAFE_CALL(pFactor,SetTranslate(kPos));
		
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_uHostID);
		if (pEnt)
		{
			SAFE_CALL(pFactor,SetRotation(pEnt->GetRotation()));
		}
		//-----------------------------------------------------------

	}

	return pFactor;
}

Kylin::Factor* Kylin::ActionDispatcher::Fire( KUINT uGID,const KPoint3& kPos )
{
	Factor* pFactor = Fire(uGID);
	SAFE_CALL(pFactor,SetTranslate(kPos));

	return pFactor;
}

Kylin::Factor* Kylin::ActionDispatcher::Fire( KUINT uGID,KUINT uTarget )
{
	Factor* pFactor = Fire(uGID);
	SAFE_CALL(pFactor,SetTarget(uTarget));

	return pFactor;
}