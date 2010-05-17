#pragma once

namespace Kylin
{
	class DataLoader
	{
	public:
		DataLoader(const KSTR& sName);
		virtual ~DataLoader();
		
		DataUnit*	GetDBPtr();
	protected:
		virtual KBOOL Initialize();
		virtual KVOID OnFinish(){}

	protected:
		friend class DataManager;

		KSTR		m_sName;
		DataUnit*	m_pDataUnit;
	};
}