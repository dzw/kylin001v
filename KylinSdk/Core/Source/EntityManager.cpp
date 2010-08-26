#include "CorePCH.h"
#include ".\entitymanager.h"
#include "entity.h"

namespace Kylin
{

	EntityManager::EntityManager(void)
	{
	}

	EntityManager::~EntityManager(void)
	{
		Destroy();
	}

	void EntityManager::Destroy()
	{
		//free all the entities;
		const EntityPool& pool = m_idManager.GetPtrs();
		for(KUINT i=0; i<pool.size(); i++)
		{
			if(pool[i] != NULL)
				EntityManager::DestroyEntity(i);
		}
	}

	Entity * EntityManager::SpawnEntity(KUINT cid)
	{
		Entity * pEnt = EntitiesFactory::CreateInstance(cid);
		if(!pEnt)
		{
			//syslog->debug("failed to create class with id:%d", cid);
			return 0;
		}

		KUINT entid = AssignID(pEnt);
		pEnt->SetID(entid);

		return pEnt;
	}

	KUINT EntityManager::AssignID(Entity * pEnt)
	{
		return m_idManager.AssignID(pEnt);
	}

	void EntityManager::DestroyEntity(KUINT id)
	{
		Entity * pEnt = m_idManager.GetPtr(id);
		assert(pEnt);
		if(pEnt)
		{
			SAFE_DEL(pEnt);
			m_idManager.RemoveID(id);
		}
	}

	void EntityManager::FillEntityPool(EntityPool & pool) const
	{
		KUINT nSize = m_idManager.GetPtrs().size();
		for(KUINT i=0; i<nSize; i++)
		{
			Entity * pEnt = m_idManager.GetPtrs()[i];
			if(pEnt)
				pool.push_back(pEnt);
		}
	}

	KVOID EntityManager::Tick( KFLOAT fElapsed )
	{
		for(KUINT i=0; i<m_idManager.GetPtrs().size(); i++)
		{
			Entity * pEnt = m_idManager.GetPtrs()[i];

			SAFE_CALL(pEnt,Tick(fElapsed));
		}
	}

	KUINT EntityManager::GetEntitiesCount( KUINT uType /*= -1*/ )
	{
		return m_idManager.GetPtrs().size();
	}
}