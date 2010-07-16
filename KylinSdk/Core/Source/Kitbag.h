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

		protected:
			Kitbag* m_pHost;
		};

		// 添加一个道具
		KBOOL	AddItem(ItemCell* pItem);
		
		// 使用道具
		KVOID	UseItem(KINT nIndex);
		
		// 掉落道具
		KVOID	ThrowItemByIndex(KINT nIndex);

		KVOID	ThrowItemByGID(KUINT uGid);

		// 删除并销毁该道具
		KVOID	RemoveAndDestroyItem(KINT nIndex);
		
		KVOID	RemoveItem(ItemCell* pCell);

		// 是否存在该编号的道具
		KINT	HasItem(KUINT uID);

		// 是否存在空余位置
		KINT	HasFreeCell();
		
		// 获得背包上限
		KINT	GetSize() { return m_uUpperLimit; }
		KVOID	SetSize(KINT uSize) { m_uUpperLimit = uSize; }
		
		// 
		Character*	GetHostChar();

	protected:
		Character*		m_pHostChar;

		// 背包大小上限
		KUINT			m_uUpperLimit;
		
		// 背包存储
		KVEC<ItemCell*>	m_kItemArray;
	};
}