#pragma once

#include "Singleton.h"

namespace Kylin
{
	class DataManager : public Singleton<DataManager>
	{
	public:
		DataManager();
		virtual ~DataManager();
		
		DataLoader* GetLoaderPtr(KSTR sName);
		
		KVOID		InvokeLoader(DataLoader* pLoader);
		KVOID		Destroy();
		KVOID		RegisterLoader(DataLoader* pLoader);
		KVOID		UnregisterLoader(DataLoader* pLoader);

	protected:
		typedef KMAP<KSTR,DataLoader*> LoaderMap;

		LoaderMap m_kLoaderMap;
	};
}