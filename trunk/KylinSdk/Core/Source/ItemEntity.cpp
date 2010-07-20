#include "corepch.h"
#include "itementity.h"
#include "RegisterClass.h"
#include "KylinRoot.h"
#include "RemoteEvents.h"
#include "ItemCell.h"
#include "Character.h"
#include "Kitbag.h"
#include "EffectManager.h"


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
		
		this->GetEntityPtr()->setQueryFlags(KylinRoot::KR_ITEM_MASK);

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
			ItemCell* pItem	 = KNEW ItemCell(pChar->GetKitbag());
			
			KUINT uType = INVALID_ID;
			m_kProperty.GetUIntValue("$GID",pItem->m_uGID);
			m_kProperty.GetUIntValue("$Type",uType);
			m_kProperty.GetUIntValue("$Belong",pItem->m_uBelong);

			pItem->m_eType = (ITEM_TYPE)uType;
			
			m_kProperty.GetStrValue("$Mesh",pItem->m_sMesh);
			m_kProperty.GetStrValue("$Icon",pItem->m_sIcon);
			m_kProperty.GetStrValue("$Explain", pItem->m_sExplain);

			pChar->GetKitbag()->AddItem(pItem);
		}

		//
		PostDestroy();
	}

	KVOID ItemEntity::BindEffect( PropertySet kProp )
	{
		//////////////////////////////////////////////////////////////////////////
		// 加载特效
		KSTR sEffect;
		if (kProp.GetStrValue("$SpawnEffect",sEffect))
		{
			EffectObject* pObj = AttachEffect(sEffect,_ITEM_DESTROY_TIME_);
			if (pObj)
			{	// 设置特效缩放
				KFLOAT fEfScale;
				if (kProp.GetFloatValue("$SpawnEffectScale",fEfScale))
				{
					pObj->SetScale(fEfScale);
				}
			}
		}
	}

	KVOID ItemEntity::PostSpawn()
	{
		// 激活特效
		KSTR sEffect;
		if (m_kProperty.GetStrValue("$SpawnEffect",sEffect))
			ActivateEffect(sEffect,true);
	}

	KVOID ItemEntity::PostDestroy()
	{
		Entity::PostDestroy();
	}
}

