#include "corepch.h"
#include "Kitbag.h"
#include "ItemCell.h"
#include "RegisterClass.h"
#include "KylinRoot.h"


Kylin::Kitbag::Kitbag(Character* pHost)
: m_uUpperLimit(1)
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

KVOID Kylin::Kitbag::FlopItem( KINT nIndex )
{
	if (nIndex < m_kItemArray.size() && nIndex >= 0)
	{
		ItemCell* pItem = m_kItemArray[nIndex];
		
		KylinRoot::GetSingletonPtr()->SpawnItem(pItem->GetGID(),id_item);
	}
}

//////////////////////////////////////////////////////////////////////////
KVOID Kylin::Kitbag::KitbagListener::OnUsed( KINT nIndex )
{
	Assert(m_pHost);
	SAFE_CALL(m_pHost,UseItem(nIndex));
}