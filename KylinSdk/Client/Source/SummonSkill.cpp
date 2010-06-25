#include "cltpch.h"
#include "SummonSkill.h"


namespace Kylin
{


	SummonSkill::SummonSkill( ActionDispatcher* pDispatcher )
		: ActSkill(pDispatcher)
	{

	}

	KBOOL SummonSkill::Init( const PropertySet& kProp )
	{
		if (!ActSkill::Init(kProp))
			return false;

		return true;
	}

	KVOID SummonSkill::Tick( KFLOAT fElapsed )
	{
		ActSkill::Tick(fElapsed);

	}

	KVOID SummonSkill::Destroy()
	{
		
		//////////////////////////////////////////////////////////////////////////
		ActSkill::Destroy();
	}

	Factor* SummonSkill::SpawnFactor()
	{
		//-----------------------------------------------------
		Factor* pFactor = ActSkill::SpawnFactor();
// 		if (BtIsKindof())
// 		{
// 		}

		//-----------------------------------------------------
		return pFactor;
	}

	KVOID SummonSkill::OnTriggered( Factor* pFactor )
	{

	}

}