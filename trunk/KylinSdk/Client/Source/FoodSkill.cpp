#include "cltpch.h"
#include "FoodSkill.h"
#include "ActionDispatcher.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "RemoteEvents.h"


Kylin::FoodSkill::FoodSkill( ActionDispatcher* pDispatcher )
: Action(pDispatcher)
{

}

KVOID Kylin::FoodSkill::OnTriggered( Factor* pFactor )
{
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(GetHostWorldID());
	Assert(pEnt);

	if (pEnt)
	{
		EventPtr spEV(
			KNEW Event(
			&ev_restore_hp, 
			Event::ev_immediate, 
			0, 
			1, 
			EventArg(GetMaxDamage())
			));

		KylinRoot::GetSingletonPtr()->PostMessage(GetHostWorldID(),spEV);
	}
}

