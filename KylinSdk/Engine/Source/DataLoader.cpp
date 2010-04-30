#include "engpch.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "DataItem.h"


Kylin::DataLoader::DataLoader()
{
	m_pDataUnit = KNEW DataUnit();

	Kylin::DataManager::GetSingletonPtr()->RegisterLoader(this);
}

Kylin::DataLoader::~DataLoader()
{
	Kylin::DataManager::GetSingletonPtr()->UnregisterLoader(this);

	SAFE_DEL(m_pDataUnit);
}