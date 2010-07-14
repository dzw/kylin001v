#include "corepch.h"
#include "itementity.h"
#include "RegisterClass.h"
#include "KylinRoot.h"
#include "RemoteEvents.h"
#include "ItemCell.h"
#include "Character.h"
#include "Kitbag.h"


#define _ITEM_DESTROY_TIME_		5.0f

namespace Kylin
{
	BtImplementRTTI(ItemEntity, Entity, id_item);

	Implement_Event_Handler(ItemEntity, Entity)
	{
		{NULL, NULL}
	};

	ItemEntity::ItemEntity()
	{

	}

	KBOOL ItemEntity::Init( const PropertySet& kProp )
	{
		if (!Entity::Init(kProp))
			return false;

		// send destroy for some time
		EventPtr spEV(
			KNEW Event(
			&ev_post_destroy, 
			Event::ev_timing, 
			_ITEM_DESTROY_TIME_, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);

		return true;
	}

	KVOID ItemEntity::BeCollected( KUINT uEntID )
	{
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		if (BtIsKindOf(Character,pEnt))
		{
			Character* pChar = BtStaticCast(Character,pEnt);
			ItemCell* pItem	 = KNEW ItemCell();
			
			//pItem... = ...
			pChar->GetKitbag()->AddItem(pItem);
		}

		//
		PostDestroy();
	}
}

