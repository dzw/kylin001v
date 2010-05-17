#pragma once

#include "Singleton.h"
#include "RegisterClass.h"


namespace Kylin
{
	class KylinHelper : public Singleton<KylinHelper>
	{
	public:
		
		Entity* SpawnCharactor(KUINT uGid, ClassID uCid);
		
	};
}