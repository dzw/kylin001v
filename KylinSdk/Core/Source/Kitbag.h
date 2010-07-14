#pragma once


namespace Kylin
{
	class Kitbag
	{
	public:
		Kitbag();
		
		class KitbagListener
		{
		public:
			KitbagListener(Kitbag* pHost)
				: m_pHost(pHost)
			{
			}
			
			virtual KVOID OnUsed(KINT nIndex);

		protected:
			Kitbag* m_pHost;
		};

		// ����һ������
		KBOOL	AddItem(ItemCell* pItem);
		
		// ʹ�õ���
		KVOID	UseItem(KINT nIndex);

		// ɾ�������ٸõ���
		KVOID	RemoveAndDestroyItem(KINT nIndex);
		
		// �Ƿ���ڸñ�ŵĵ���
		KINT	HasItem(KUINT uID);

		// �Ƿ���ڿ���λ��
		KINT	HasFreeCell();
		
		// ��ñ�������
		KINT	GetSize() { return m_uUpperLimit; }
		KVOID	SetSize(KINT uSize) { m_uUpperLimit = uSize}

	protected:
		
		// ������С����
		KUINT			m_uUpperLimit;
		
		// �����洢
		KVEC<ItemCell*>	m_kItemArray;
	};
}