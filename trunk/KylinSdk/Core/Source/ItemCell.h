#pragma once

namespace Kylin
{
	class ItemCell
	{
	public:
		ItemCell();
		
		enum StorageType
		{
			ST_ALONE,
			ST_DOZEN,
		};
		
		// 使用该道具
		KVOID			UseTheItem();

		// 获得道具存储方式
		StorageType		GetStorageType();
		KUINT			GetGID(){ return m_uGID; }

	protected:
		friend class Kitbag;

		// 数量上限
		KUINT		m_uLimit;

		// 数量
		KINT		m_nCount;
		
		// 道具ID
		KUINT		m_uGID;
		
		// 存储方式
		StorageType	m_eStorageType;

		// UI图标
		KSTR		m_sIcon;

	};
}