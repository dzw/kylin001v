#include "cltpch.h"
#include "TeleportSkill.h"
#include "KylinRoot.h"
#include "RemoteEvents.h"
#include "Entity.h"


namespace Kylin
{
	TeleportSkill::TeleportSkill( ActionDispatcher* pDispatcher )
		: ActSkill(pDispatcher)
	{

	}

	KBOOL TeleportSkill::Init( const PropertySet& kProp )
	{
		if (!ActSkill::Init(kProp))
			return false;

		return true;
	}

	KVOID TeleportSkill::Tick( KFLOAT fElapsed )
	{
		ActSkill::Tick(fElapsed);

	}

	KVOID TeleportSkill::Destroy()
	{

		//////////////////////////////////////////////////////////////////////////
		ActSkill::Destroy();
	}

	Factor* TeleportSkill::SpawnFactor()
	{
		//-----------------------------------------------------
		Factor* pFactor = ActSkill::SpawnFactor();

		//-----------------------------------------------------
		return pFactor;
	}

	KVOID TeleportSkill::OnTriggered( Factor* pFactor )
	{

		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(this->GetHostWorldID());
		if (pEnt)
		{
			KANY var;
			if (pEnt->GetPropertyRef().GetValue("$BirthPosition",var))
			{
				KPoint3 kPos = boost::any_cast<KPoint3>(var);
		
				EventPtr spEV(
					KNEW Event(
					&ev_do_teleport, 
					Event::ev_immediate, 
					0, 
					2, 
					EventArg(kPos.x),
					EventArg(kPos.z)
					));

				KylinRoot::GetSingletonPtr()->PostMessage(GetHostWorldID(),spEV);
			}
		}
	}

}