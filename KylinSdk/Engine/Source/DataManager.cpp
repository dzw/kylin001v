#include "engpch.h"
#include "DataManager.h"
#include "DataLoader.h"
#include "DataItem.h"


#define DB_GLOBAL "Data/db/global.csv"

Kylin::DataManager::DataManager()
{
	Initialize();
}

Kylin::DataManager::~DataManager()
{
	Destroy();
}

Kylin::DataLoader* Kylin::DataManager::GetLoaderPtr( KSTR sName )
{
	LoaderMap::iterator it = m_kLoaderMap.find(sName);
	if (it != m_kLoaderMap.end())
	{
		return it->second;
	}

	return NULL;
}

KVOID Kylin::DataManager::RegisterLoader( DataLoader* pLoader )
{
	if (!pLoader) return;

	LoaderMap::iterator it = m_kLoaderMap.find(pLoader->m_sName);
	if (it != m_kLoaderMap.end())
	{
		SAFE_DEL(pLoader);
	}
	else
	{
		m_kLoaderMap.insert(std::pair<KSTR,DataLoader*>(pLoader->m_sName,pLoader));
	}
}

KVOID Kylin::DataManager::UnregisterLoader( DataLoader* pLoader )
{
	LoaderMap::iterator it = m_kLoaderMap.find(pLoader->m_sName);
	if (it != m_kLoaderMap.end())
	{
		m_kLoaderMap.erase(it);
	}
}

KVOID Kylin::DataManager::Destroy()
{
	LoaderMap::iterator beg = m_kLoaderMap.begin();
	LoaderMap::iterator end = m_kLoaderMap.end();

	for (LoaderMap::iterator it = beg; it != end; it++)
	{
		SAFE_DEL(it->second);
	}

	m_kLoaderMap.clear();
}

KVOID Kylin::DataManager::InvokeLoader( DataLoader* pLoader )
{
	if (!pLoader) return;

	LoaderMap::iterator it = m_kLoaderMap.find(pLoader->m_sName);
	if (it != m_kLoaderMap.end())
	{
		pLoader->Initialize();
	}
}

KBOOL Kylin::DataManager::GetGlobalValue( KSTR sKey, KSTR& sValue )
{
	Kylin::DataLoader* pLoader = this->GetLoaderPtr(DB_GLOBAL);
	
	QResult kRes = pLoader->GetDBPtr()->Query("KEY",Kylin::DataUnit::DF_EQUAL,sKey);
	if (kRes.size() == 0)
		return false;
	
	assert(kRes.size() == 1);
	Kylin::DataItem* pDBItem = kRes[0];

	DataItem::DataField dbField;
	pDBItem->QueryField("VALUE",dbField);
	sValue = boost::any_cast<KSTR>(dbField.m_aValue);
	
	return true;
}

KVOID Kylin::DataManager::Initialize()
{
	InvokeLoader(KNEW DataLoader(DB_GLOBAL));

}

KBOOL Kylin::DataManager::Select( KCSTR& sTable,const KUINT& kId,KCSTR& sField,KANY& aRet )
{
	KSTR sValue;
	if (!GetGlobalValue(sTable,sValue))
		return false;

	Kylin::DataLoader* pLoader = GetLoaderPtr(sValue);
	// 查询对应的因子信息
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(kId,dbItem))
		return false;

	DataItem::DataField dbField;
	if (dbItem.QueryField(sField,dbField))
	{
		aRet = dbField.m_aValue;
		return true;
	}

	return false;
}
