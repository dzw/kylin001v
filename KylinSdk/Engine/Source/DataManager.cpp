#include "engpch.h"
#include "DataManager.h"
#include "DataLoader.h"


Kylin::DataManager::DataManager()
{

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