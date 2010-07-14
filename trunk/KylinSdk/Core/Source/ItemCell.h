#pragma once

//tolua_begin
enum ITEM_TYPE
{
	IT_AVATAR,
	IT_ACTION,
};
//tolua_end

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
		
		// ʹ�øõ���
		KVOID			UseTheItem();

		// ��õ��ߴ洢��ʽ
		StorageType		GetStorageType();
		KUINT			GetGID(){ return m_uGID; }

	protected:
		friend class Kitbag;
		friend class ItemEntity;

		// ��������
		KUINT		m_uLimit;

		// ����
		KINT		m_nCount;
		
		// ����ID
		KUINT		m_uGID;
		
		// ��������
		ITEM_TYPE	m_eType;

		// �洢��ʽ
		StorageType	m_eStorageType;

		// UIͼ��
		KSTR		m_sIcon;

	};
}