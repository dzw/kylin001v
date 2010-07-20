#pragma once

namespace Kylin
{
	class Kitbag
	{
	public:
		Kitbag(Character* pHost);
		~Kitbag();

		class KitbagListener
		{
		public:
			KitbagListener(Kitbag* pHost)
				: m_pHost(pHost)
			{
			}
			
			virtual KVOID OnUsed(KINT nIndex);
			
			Kitbag* GetKitbag();

		protected:
			Kitbag* m_pHost;
		};

		// ���һ������
		KBOOL	AddItem(ItemCell* pItem);
		KBOOL	AddItem(KUINT uGID);

		// ʹ�õ���
		KVOID	UseItem(KINT nIndex);
		
		// �������
		KVOID	ThrowItemByIndex(KINT nIndex);

		KVOID	ThrowItemByGID(KUINT uGid);

		// ɾ�������ٸõ���
		KVOID	RemoveAndDestroyItem(KINT nIndex);
		
		KVOID	RemoveItem(ItemCell* pCell);

		// �Ƿ���ڸñ�ŵĵ���
		KINT	HasItem(KUINT uID);

		// �Ƿ���ڿ���λ��
		KINT	HasFreeCell();
		
		// ��ñ�������
		KINT	GetSize() { return m_uUpperLimit; }
		KVOID	SetSize(KINT uSize) { m_uUpperLimit = uSize; }
		
		// 
		Character*	GetHostChar();
		
		const KVEC<ItemCell*> GetItems() { return m_kItemArray; }

	protected:
		Character*		m_pHostChar;

		// ������С����
		KUINT			m_uUpperLimit;
		
		// �����洢
		KVEC<ItemCell*>	m_kItemArray;
	};
}