#include "cltpch.h"
#include "LearnSkill.h"
#include "ActionDispatcher.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "RemoteEvents.h"


Kylin::LearnSkill::FoodSkill( ActionDispatcher* pDispatcher )
: Action(pDispatcher)
{

}

KVOID Kylin::LearnSkill::OnTriggered( Factor* pFactor )
{
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(GetHostWorldID());
	Assert(pEnt);

	if (pEnt)
	{
		EventPtr spEV(
			KNEW Event(
			&ev_learn_skill, 
			Event::ev_immediate, 
			0, 
			1, 
			EventArg(GetMaxDamage())
			));

		KylinRoot::GetSingletonPtr()->PostMessage(GetHostWorldID(),spEV);
	}
}

