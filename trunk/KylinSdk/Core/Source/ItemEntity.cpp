#include "corepch.h"
#include "itementity.h"


Kylin::ItemEntity::ItemEntity()
{

}

KVOID Kylin::ItemEntity::BeCollected( KUINT uEntID )
{
	// Kylin::Entity* pEnt = GetEntity(uEntID);
	// if (pEnt)
	// {
	//		ItemCell* pItem = ItemCell(pEnt->GetKitbagPtr());
	//		pItem... = ...
	//		pEnt->GetKitbagPtr()->AddItem(pItem);
	// }

	// send destroy for some time
}
