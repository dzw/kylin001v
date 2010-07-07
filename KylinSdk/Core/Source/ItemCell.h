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
		
		// ʹ�øõ���
		KVOID			UseTheItem();

		// ��õ��ߴ洢��ʽ
		StorageType		GetStorageType();
		KUINT			GetGID(){ return m_uGID; }

	protected:
		friend class Kitbag;

		// ��������
		KUINT		m_uLimit;

		// ����
		KINT		m_nCount;
		
		// ����ID
		KUINT		m_uGID;
		
		// �洢��ʽ
		StorageType	m_eStorageType;

		// UIͼ��
		KSTR		m_sIcon;

	};
}