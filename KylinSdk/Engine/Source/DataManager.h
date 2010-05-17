#pragma once

#include "Singleton.h"

namespace Kylin
{

	class DataManager : public Singleton<DataManager>
	{
	public:
		DataManager();
		virtual ~DataManager();
		
		KVOID		Initialize();

		DataLoader* GetLoaderPtr(KSTR sName);
		
		KVOID		InvokeLoader(DataLoader* pLoader);
		KVOID		Destroy();
		KVOID		RegisterLoader(DataLoader* pLoader);
		KVOID		UnregisterLoader(DataLoader* pLoader);
		
		KBOOL		GetGlobalValue(KSTR sKey, KSTR& sValue);

	protected:
		typedef KMAP<KSTR,DataLoader*> LoaderMap;

		LoaderMap m_kLoaderMap;
	};
}