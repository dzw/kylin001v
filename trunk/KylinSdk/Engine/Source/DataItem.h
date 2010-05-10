#pragma once

namespace Kylin
{
	class DataItem
	{
	public:
		
		class DataField
		{
		public:
			KSTR m_sKey;
			KSTR m_sType;
			KANY m_aValue;
		};
		
		KBOOL QueryField(const KSTR& sKey, DataField& kDF)
		{
			KMAP<KSTR,DataField>::iterator it = m_kFieldsMap.find(sKey);
			if (it != m_kFieldsMap.end())
			{
				kDF = it->second;
				return true;
			}

			return false;
		}

		KVOID AddField(DataField kField)
		{
			DataField kDF;
			if (!QueryField(kField.m_sKey,kDF))
				m_kFieldsMap.insert(std::pair<KSTR,DataField>(kField.m_sKey,kField));
		}

		KVOID DelField(const KSTR& sKey)
		{
			KMAP<KSTR,DataField>::iterator it = m_kFieldsMap.find(sKey);
			if (it != m_kFieldsMap.end())
			{
				m_kFieldsMap.erase(it);
			}
		}

	protected:
		KMAP<KSTR,DataField> m_kFieldsMap;
	};
	
	typedef KVEC<DataItem*> QResult;
	
	class DataUnit
	{
	public:
		
		KBOOL ReadCSV(KCCHAR* sCvs)
		{
			ifstream fsInFile (sCvs);
			if(!fsInFile.is_open())
				return false;
		
			m_kItemsMap.clear();

			KSTR sLine;
			KINT nLineNum = 0;
			while (getline (fsInFile, sLine))
			{
				nLineNum++;
			
				istringstream kLinestream(sLine);
				KSTR sItem;
				KINT nItemNum = 0;
				DataItem kDataItem;
				while (getline (kLinestream, sItem, ','))
				{
					nItemNum++;
					
					// 数据表中第一行为解释说明部分
					if (nLineNum == 1) 
					{
						m_kKeyTable.push_back(sItem);
						continue;
					}
					// 数据表中第二行为类型定义部分
					else if (nLineNum == 2)
					{
						m_kTypeTable.push_back(sItem);
						continue;
					}
					
					DataItem::DataField kField;
					kField.m_sKey	= m_kKeyTable[nItemNum-1];
					kField.m_sType	= m_kTypeTable[nItemNum-1];

					if (kField.m_sType == "INT")
						kField.m_aValue = atoi(sItem.data());
					else if (kField.m_sType == "FLOAT")
						kField.m_aValue = (KFLOAT)atof(sItem.data());
					else if (kField.m_sType == "STRING")
						kField.m_aValue = sItem;
					else if (kField.m_sType == "BOOL")
						kField.m_aValue = (atoi(sItem.data()) != 0);
					else 
					{
						assert(NULL);
						continue;
					}

					kDataItem.AddField(kField);
				}

				AddItem(kDataItem);
			}

			return true;
		}

		enum DFactor
		{
			DF_LESS  = -1,
			DF_EQUAL = 0,
			DF_MORE  = 1,
		};

		KVOID AddItem(DataItem kItem)
		{
			DataItem::DataField kDF;
			if (kItem.QueryField("ID",kDF))
			{
				KUINT uID = boost::any_cast<KINT>(kDF.m_aValue);
				if (m_kItemsMap.find(uID) == m_kItemsMap.end())
					m_kItemsMap.insert(std::pair<KUINT,DataItem>(uID,kItem));
			}
		}

		KVOID DelItem(KUINT uID)
		{
			KMAP<KUINT,DataItem>::iterator it = m_kItemsMap.find(uID);
			if (it != m_kItemsMap.end())
				m_kItemsMap.erase(it);
		}

		QResult Query(const KSTR& sKey, DFactor eFactor, const KANY& aValue)
		{
			QResult vRet;
			
			KMAP<KUINT,DataItem>::iterator beg = m_kItemsMap.begin();
			KMAP<KUINT,DataItem>::iterator end = m_kItemsMap.end();
			KMAP<KUINT,DataItem>::iterator it  = beg;
			
			DataItem* kItem;
			DataItem::DataField kDF;
			for (; it != end ; it++)
			{
				kItem = &(it->second);

				if (kItem->QueryField(sKey,kDF))
				{
					switch (eFactor)
					{
					case DF_EQUAL:
						
						if (kDF.m_sType == "STRING")
						{
							KSTR sVar1 = boost::any_cast<KSTR>(kDF.m_aValue);
							KSTR sVar2 = boost::any_cast<KSTR>(aValue);

							if (sVar1 == sVar2)
								vRet.push_back(kItem);
						}
						else if(kDF.m_sType == "INT")
						{
							KINT nVar1 = boost::any_cast<KINT>(kDF.m_aValue);
							KINT nVar2 = boost::any_cast<KINT>(aValue);
							
							if (nVar1 == nVar2)
								vRet.push_back(kItem);
						}
						else if (kDF.m_sType == "FLOAT")
						{
							KFLOAT nVar1 = boost::any_cast<KFLOAT>(kDF.m_aValue);
							KFLOAT nVar2 = boost::any_cast<KFLOAT>(aValue);

							if (nVar1 == nVar2)
								vRet.push_back(kItem);
						}
						else if (kDF.m_sType == "BOOL")
						{
							KBOOL nVar1 = boost::any_cast<KBOOL>(kDF.m_aValue);
							KBOOL nVar2 = boost::any_cast<KBOOL>(aValue);

							if (nVar1 == nVar2)
								vRet.push_back(kItem);
						}

						break;
					case DF_LESS:

						break;
					case DF_MORE:

						break;
					}

					vRet.push_back(kItem);
				}
			}

			return vRet;
		}


		KBOOL Query(KUINT uID, DataItem& kItem)
		{
			KMAP<KUINT,DataItem>::iterator it = m_kItemsMap.find(uID);
			if (it != m_kItemsMap.end())
			{
				kItem = it->second;
				return true;
			}

			return false;
		}

	protected:
		KVEC<KSTR>			 m_kKeyTable;
		KVEC<KSTR>			 m_kTypeTable;
		KMAP<KUINT,DataItem> m_kItemsMap;
	};
}
