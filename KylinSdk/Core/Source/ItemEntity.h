#pragma once

#include "Entity.h"

namespace Kylin
{
	class ItemEntity : public Entity
	{
	public:
		ItemEntity();
	
	public:
		virtual KVOID	BeCollected(KUINT uEntID);

	protected:

	};
}