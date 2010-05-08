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
// 	QResult aRes = m_pDataUnit->Query("类型",DataUnit::DF_EQUAL,aValue);
// 	
// 	for (KUINT i = 0; i < aRes.size(); i++)
// 	{
// 		DataItem* pItem = aRes[i];
// 		DataItem::DataField kNameDF;
// 		pItem->QueryField("名称",kNameDF);
// 		
// 		KSTR sName = kNameDF.m_aValue;
// 
// 		m_kCharBaseProp.SetValue("$Name",sName);
// 
// 		DataItem::DataField kModelDF;
// 		pItem->QueryField("模型",kModelDF);
// 		
// 		KSTR sModel = kModelDF.m_aValue;
// 
// 		m_kCharBaseProp.SetValue("$Model",sModel);
// 		
// 		DataItem::DataField kExpDF;
// 		pItem->QueryField("模型",kExpDF);
// 
// 		KSTR sExp = kExpDF.m_aValue;
// 		
// 		m_kCharBaseProp.SetValue("$Exp",sExp);
// 	}
// }