#include "corepch.h"
#include "Kitbag.h"



Kylin::Kitbag::Kitbag()
: m_uUpperLimit(1)
{

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
			m_kItemArray[n].m_nCount++;
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
		m_kItemArray[i]->UseTheItem();
	}
}
//////////////////////////////////////////////////////////////////////////
KVOID Kylin::Kitbag::KitbagListener::OnUsed( KINT nIndex )
{
	Assert(m_pHost);
	SAFE_CALL(m_pHost,UseItem(nIndex));
}