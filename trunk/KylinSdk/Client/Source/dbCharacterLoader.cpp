#include "cltpch.h"
#include "dbCharacterLoader.h"
#include "DataItem.h"


Kylin::CharacterLoader::~CharacterLoader()
{

}
// 
// KVOID Kylin::CharacterLoader::OnFinish()
// {
// 	
// 	KANY aValue = 1;
// 	QResult aRes = m_pDataUnit->Query("����",DataUnit::DF_EQUAL,aValue);
// 	
// 	for (KUINT i = 0; i < aRes.size(); i++)
// 	{
// 		DataItem* pItem = aRes[i];
// 		DataItem::DataField kNameDF;
// 		pItem->QueryField("����",kNameDF);
// 		
// 		KSTR sName = kNameDF.m_aValue;
// 
// 		m_kCharBaseProp.SetValue("$Name",sName);
// 
// 		DataItem::DataField kModelDF;
// 		pItem->QueryField("ģ��",kModelDF);
// 		
// 		KSTR sModel = kModelDF.m_aValue;
// 
// 		m_kCharBaseProp.SetValue("$Model",sModel);
// 		
// 		DataItem::DataField kExpDF;
// 		pItem->QueryField("ģ��",kExpDF);
// 
// 		KSTR sExp = kExpDF.m_aValue;
// 		
// 		m_kCharBaseProp.SetValue("$Exp",sExp);
// 	}
// }