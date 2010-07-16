#pragma once

#include "Entity.h"

namespace Kylin
{
	class ItemEntity : public Entity
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(ItemEntity, Entity)
		Declare_Entity_Creator(ItemEntity)

		ItemEntity();
	
	public:
		virtual KVOID	BeCollected(KUINT uEntID);

	protected:
		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
		virtual KVOID BindEffect(PropertySet kProp);
	};
}