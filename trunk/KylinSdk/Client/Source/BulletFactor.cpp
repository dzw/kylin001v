#include "cltpch.h"
#include "BulletFactor.h"
#include "clRegisterClass.h"

namespace Kylin
{
	BtImplementRTTI(BulletFactor, Factor, id_bullet_factor);

	Implement_Event_Handler(BulletFactor, Factor)
	{
		{NULL, NULL}
	};

	BulletFactor::BulletFactor()
	{

	}

	KBOOL BulletFactor::Init( const PropertySet& kProp )
	{
		if ( !Factor::Init(kProp) )
			return false;
		
		
		return true;
	}

	KVOID BulletFactor::Tick( KFLOAT fElapsed )
	{
		Factor::Tick(fElapsed);


	}

	KVOID BulletFactor::PostSpawn()
	{
		Factor::PostSpawn();

	}

	KVOID BulletFactor::PostDestroy()
	{
		Factor::PostDestroy();

	}
}

