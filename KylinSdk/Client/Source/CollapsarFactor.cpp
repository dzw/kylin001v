#include "cltpch.h"
#include "CollapsarFactor.h"
#include "ClRegisterClass.h"


namespace Kylin
{
	BtImplementRTTI(CollapsarFactor, Factor, id_collapsar_factor);

	Implement_Event_Handler(CollapsarFactor, Factor)
	{
		{NULL, NULL}
	};

	CollapsarFactor::CollapsarFactor()
	{

	}

	KBOOL CollapsarFactor::Init( const PropertySet& kProp )
	{
		if (!Factor::Init(kProp))
			return false;

		return true;
	}

	KVOID CollapsarFactor::Tick( KFLOAT fElapsed )
	{
		Factor::Tick(fElapsed);


	}

	KVOID CollapsarFactor::BindEffect( PropertySet kProp )
	{

	}

	KVOID CollapsarFactor::PostSpawn()
	{
		Factor::PostSpawn();

	}

	KVOID CollapsarFactor::PostDestroy()
	{


		Factor::PostDestroy();
	}

	KVOID CollapsarFactor::EndTime( KCSTR& sClass,KCSTR& sName, KANY aUserData )
	{

	}
}

