#pragma once

namespace Kylin
{
	class DataLoader
	{
	public:
		DataLoader();
		virtual ~DataLoader();

		virtual KBOOL Initialize() = 0;
		virtual KVOID OnLoading() = 0;
		virtual KVOID OnFinish() = 0;

	protected:
		friend class DataManager;

		KSTR		m_sName;
		DataUnit*	m_pDataUnit;
	};
}