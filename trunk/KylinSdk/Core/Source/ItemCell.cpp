#include "corepch.h"
#include "ItemCell.h"
#include "Kitbag.h"
#include "Character.h"
#include "ActionDispatcher.h"


Kylin::ItemCell::ItemCell(Kitbag* pKitbag)
: m_pKitbag(pKitbag)
, m_uGID(INVALID_ID)
, m_uBelong(INVALID_ID)
, m_uEffectID(INVALID_ID)
, m_nCount(1)
, m_uLimit(1)
, m_eStorageType(ST_ALONE)
{

}

KVOID Kylin::ItemCell::UseTheItem()
{
	assert(m_nCount > 0);
	
	// active item by id
	if (m_eType == IT_ACTION)
	{
		SAFE_CALL(m_pKitbag->GetHostChar()->GetActionDispatcher(),Fire(m_uBelong));
	}
	else if (m_eType == IT_AVATAR)
	{
		// »»×°
	}

	m_nCount--;
	if (!m_nCount)
	{
		// remove and destroy this item
		m_pKitbag->RemoveItem(this);
	}
}

Kylin::ItemCell::StorageType Kylin::ItemCell::GetStorageType()
{
	return m_eStorageType;
}
