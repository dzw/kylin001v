#include "corepch.h"
#include "ItemCell.h"

Kylin::ItemCell::ItemCell()
: m_uGID(INVALID_ID)
, m_nCount(1)
, m_uLimit(1)
, m_eStorageType(ST_ALONE)
{

}

KVOID Kylin::ItemCell::UseTheItem()
{
	assert(m_nCount > 0);
	
	// active item by id
	

	m_nCount--;
	if (!m_nCount)
	{
		// remove and destroy this item
	}
}

Kylin::ItemCell::StorageType Kylin::ItemCell::GetStorageType()
{
	return m_eStorageType;
}
