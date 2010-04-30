#pragma once

namespace Kylin
{
	template<class T>
	class IDManager
	{
	public:
		IDManager(KINT nInitialSize = 512)
		{
			SetSize(nInitialSize);
		};

		~IDManager(){};

		//Allocate a new id for this pointer
		KUINT AssignID(T * pointer)
		{
			//check if the idpool is half empty
			if(m_dIDPool.size() <= (m_dTable.size() >> 1) )
			{
				Expand();
			}

			//assign the tail of the deque as id
			KINT id = m_dIDPool.back();
			m_dIDPool.pop_back();

			//insert the id and pointer to the array
			m_dTable[id] = pointer;
			return id;
		};

		//Remove the pointer with id from the manager, recycle the id after pop up the verify part
		void RemoveID(KUINT id)
		{
			//check id validaty
			if(id >= (KUINT)(m_dTable.size()) || id < 0)
				return;

			//set the ptr to null
			m_dTable[id] = NULL;

			//recycle the id
			m_dIDPool.push_front( id);
		}

		//Get the pointer with id
		T * GetPtr(KUINT id)
		{
			//check id validaty
			if(id >= (KUINT)m_dTable.size() || id < 0)
				return NULL;

			//return the pointer at index of id
			return m_dTable[id];
		}

		//debug print all the id array
		void DebugPrintTableStatus()
		{
			printf("IDManager table status\n");
			for(KUINT64 i=0; i<m_dTable.size(); i++)
				printf(m_dTable[i] == NULL? "*" : "-");
		}

		const std::vector<T*>& GetPtrs() const {return m_dTable;}

	private:
		//Set the new size of the id table
		void SetSize(KINT nSize)
		{
			KINT nOldSize = static_cast<KINT>(m_dTable.size());
			if(nOldSize >= nSize)
				return;

			m_dTable.resize(nSize);

			for(KINT i=nOldSize; i< nSize; i++)
			{
				m_dIDPool.push_front( i );
				m_dTable[i] = NULL;
			}
		}

		//expand the table size to allocate more ids.
		void Expand()
		{
			SetSize(static_cast<KINT>(m_dTable.size() * 2));
		}

		std::deque<KINT> m_dIDPool;

		std::vector<T*> m_dTable;
	};

}
