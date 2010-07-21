#include "corepch.h"
#include "Kitbag.h"
#include "ItemCell.h"
#include "RegisterClass.h"
#include "KylinRoot.h"
#include "rOgreRoot.h"
#include "Character.h"
#include "DataItem.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "ScriptVM.h"


Kylin::Kitbag::Kitbag(Character* pHost)
: m_uUpperLimit(21)
, m_pHostChar(pHost)
{

}


Kylin::Kitbag::~Kitbag()
{
	for (KUINT i = 0; i < m_kItemArray.size(); i++)
	{
		SAFE_DEL(m_kItemArray[i]);
	}
	m_kItemArray.clear();
}

KBOOL Kylin::Kitbag::AddItem( ItemCell* pItem )
{
	if (m_kItemArray.size() >= m_uUpperLimit)
	{
		SAFE_DEL(pItem);
		return false;
	}

	if (pItem->GetStorageType() == ItemCell::ST_ALONE)
		m_kItemArray.push_back(pItem);
	else
	{
		KINT n = HasItem(pItem->GetGID());
		if (n != -1)
		{
			m_kItemArray[n]->m_nCount++;
		}
		else
		{
			m_kItemArray.push_back(pItem);
		}
	}
}

KBOOL Kylin::Kitbag::AddItem( KUINT uGID )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("ITEM_DB",sValue))
		return NULL;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// 查询对应的角色信息
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGID,dbItem))
		return false;
	
	ItemCell* pItem	 = KNEW ItemCell(this);
	//------------------------------------------------------------------
	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	pItem->m_sMesh = boost::any_cast<KSTR>(dbField.m_aValue);

	dbItem.QueryField("TYPE",dbField);
	KSTR sType = boost::any_cast<KSTR>(dbField.m_aValue);
	pItem->m_eType = (ITEM_TYPE)(KUINT)OgreRoot::GetSingletonPtr()->GetScriptVM()->GetGlobalNumber(sType.data());

	dbItem.QueryField("BELONG",dbField);
	pItem->m_uBelong = boost::any_cast<KINT>(dbField.m_aValue);

	dbItem.QueryField("ICON",dbField);
	pItem->m_sIcon = boost::any_cast<KSTR>(dbField.m_aValue);

	dbItem.QueryField("EXPLAIN",dbField);
	pItem->m_sExplain = boost::any_cast<KSTR>(dbField.m_aValue);
	
	dbItem.QueryField("EFFECT",dbField);
	pItem->m_uEffectID = boost::any_cast<KINT>(dbField.m_aValue);
	//------------------------------------------------------------------
	AddItem(pItem);	
}

KVOID Kylin::Kitbag::RemoveAndDestroyItem( KINT nIndex )
{
	if (nIndex < m_kItemArray.size() && nIndex >= 0)
	{
		SAFE_DEL(m_kItemArray[nIndex]);
		m_kItemArray.erase(m_kItemArray.begin() + nIndex);
	}
}

KINT Kylin::Kitbag::HasItem( KUINT uID )
{
	for (KUINT i = 0; i < m_kItemArray.size(); i++)
	{
		if ( m_kItemArray[i]->GetGID() == uID )
		{
			return i;
		}
	}

	return -1;
}

KINT Kylin::Kitbag::HasFreeCell()
{
	return m_uUpperLimit - m_kItemArray.size();
}

KVOID Kylin::Kitbag::UseItem( KINT nIndex )
{
	if (nIndex < m_kItemArray.size() && nIndex >= 0)
	{
		m_kItemArray[nIndex]->UseTheItem();
	}
}

KVOID Kylin::Kitbag::ThrowItemByIndex( KINT nIndex )
{
	if (nIndex < m_kItemArray.size() && nIndex >= 0)
	{
		ItemCell* pItem = m_kItemArray[nIndex];
		
// 		Entity* pEnt = KylinRoot::GetSingletonPtr()->SpawnItem(pItem->GetGID(),id_item);
// 		if (pEnt)
// 		{
// 			pEnt->SetTranslate(m_pHostChar->GetTranslate());
// 		}
	}
}

KVOID Kylin::Kitbag::ThrowItemByGID( KUINT uGid )
{
	Entity* pEnt = KylinRoot::GetSingletonPtr()->SpawnItem(uGid,id_item);
	if (pEnt)
	{
		pEnt->SetTranslate(m_pHostChar->GetTranslate());
	}
}

KVOID Kylin::Kitbag::RemoveItem( ItemCell* pCell )
{
	for (KUINT i = 0; i < m_kItemArray.size(); i++)
	{
		if (m_kItemArray[i] == pCell)
		{
			SAFE_DEL(m_kItemArray[i]);
			m_kItemArray.erase(m_kItemArray.begin() + i);
			break;
		}
	}
}

Kylin::Character* Kylin::Kitbag::GetHostChar()
{
	return m_pHostChar;
}	
//////////////////////////////////////////////////////////////////////////
KVOID Kylin::Kitbag::KitbagListener::OnUsed( KINT nIndex )
{
	Assert(m_pHost);
	SAFE_CALL(m_pHost,UseItem(nIndex));
}

Kylin::Kitbag* Kylin::Kitbag::KitbagListener::GetKitbag()
{
	return m_pHost;
}