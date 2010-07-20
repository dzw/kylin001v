#pragma once

//tolua_begin
enum ITEM_TYPE
{
	IT_AVATAR,		// װ�����ͣ������������ε�
	IT_ACTION,		// �������ͣ����Ѫ�����׵�
};
//tolua_end

namespace Kylin
{
	class ItemCell
	{
	public:
		ItemCell(Kitbag* pKitbag);
		
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

//	protected:
//		friend class Kitbag;
//		friend class ItemEntity;
		
		Kitbag*		m_pKitbag;

		// ��������
		KUINT		m_uLimit;

		// ����
		KINT		m_nCount;
		
		// ����ID
		KUINT		m_uGID;
		
		// ����ID
		KUINT		m_uBelong;
		
		// ��ЧID
		KUINT		m_uEffectID;

		// ��������
		ITEM_TYPE	m_eType;

		// �洢��ʽ
		StorageType	m_eStorageType;
		
		// UIͼ��
		KSTR		m_sIcon;
		
		// ģ��
		KSTR		m_sMesh;

		// ˵��
		KSTR		m_sExplain;
	};
}