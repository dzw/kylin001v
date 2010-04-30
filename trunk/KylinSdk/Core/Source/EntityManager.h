#pragma once
#include "IDManager.h"
#include "RegisterClass.h"
#include "Archive.h"


namespace Kylin
{
	class EntityManager
	{
	public:
		typedef KVEC<Entity *> EntityPool;

		EntityManager(void);
		virtual ~EntityManager(void);
		
		///
		virtual KVOID	 Tick(KFLOAT fElapsed);
		///Spawn a entity of the given class id;
		virtual Entity * SpawnEntity(KUINT cid );

		///Get the entity ptr with the entity id;
		virtual Entity * GetEntityPtr(KUINT id) 
		{ 
			return m_idManager.GetPtr(id);
		}

		///Destroy entity with the entity id;
		virtual KVOID	DestroyEntity(KUINT id);

		///Destroy EntityManager
		virtual	KVOID	Destroy();

		virtual KVOID	FillEntityPool(EntityPool & pool) const;
		
		///utype = -1 get all entities
		virtual KUINT	GetEntitiesCount(KUINT uType = -1);

	protected:

		virtual KUINT	AssignID(Entity * pEnt);

	private:
		IDManager<Entity>	m_idManager;
	};
}
