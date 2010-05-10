#include "engpch.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "DataItem.h"


Kylin::DataLoader::DataLoader(const KSTR& sName)
: m_sName(sName)
{
	m_pDataUnit = KNEW DataUnit();
	
	Kylin::DataManager::GetSingletonPtr()->RegisterLoader(this);
}

Kylin::DataLoader::~DataLoader()
{
	//Kylin::DataManager::GetSingletonPtr()->UnregisterLoader(this);

	SAFE_DEL(m_pDataUnit);
}

KBOOL Kylin::DataLoader::Initialize()
{
	KBOOL bRet = m_pDataUnit->ReadCSV(m_sName.data());
	
	OnFinish();

	return bRet;
}

Kylin::DataUnit* Kylin::DataLoader::GetDBPtr()
{
	return m_pDataUnit;
}