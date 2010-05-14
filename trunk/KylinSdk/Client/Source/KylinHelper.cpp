#include "cltpch.h"
#include "KylinHelper.h"
#include "DataManager.h"
#include "dbCharacterLoader.h"
#include "DataItem.h"
#include "FileUtils.h"
#include "Property.h"
#include "KylinRoot.h"


Kylin::Entity* Kylin::KylinHelper::SpawnCharactor( KUINT uGid, ClassID uCid )
{
	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(DB_CHARACTER);
	// ��ѯ��Ӧ�Ľ�ɫ��Ϣ
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGid,dbItem))
		return false;

	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);
	// ע�� ·��ǰ������ "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);
	KSTR sDir  = FileUtils::GetRelativePath(sModel);
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists("General",sName))
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");
	
	//////////////////////////////////////////////////////////////////////////
	// addResourceLocation �˷������ʽŲ��޷�������Ҫ�ֶ�����
	KSTR sMat = FileUtils::GetFileName(sModel);
	sMat += ".material";
	if (!Ogre::MaterialManager::getSingletonPtr()->resourceExists(sMat))
	{
		sMat = sDir + sMat;
		std::ifstream fs(sMat.data());
		Ogre::DataStreamPtr stream = Ogre::DataStreamPtr(OGRE_NEW Ogre::FileStreamDataStream(&fs, false));
		Ogre::MaterialManager::getSingletonPtr()->parseScript(stream,"General");
	}
	//////////////////////////////////////////////////////////////////////////

	PropertySet kProp;
	kProp.SetValue("$CLASS_ID",(KUINT)uCid);
	kProp.SetValue("$Mesh",sName);
	kProp.SetValue("$Materials",sMaterials);

	kProp.SetValue("$CLLSN_SHAPE",(KUINT)1);
	kProp.SetValue("$CLLSN_TYPE", (KUINT)0);
	kProp.SetValue("$COLLISION",true);

	Entity * pEnt = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);


	return pEnt;
}