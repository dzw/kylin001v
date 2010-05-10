#include "cltpch.h"
#include "Showgirl.h"
#include "DataItem.h"
#include "DataManager.h"
#include "dbCharacterLoader.h"
#include "FileUtils.h"

Kylin::Showgirl::Showgirl(KUINT uGid)
: m_uGid(uGid)
, m_uCurAnimIndex(-1)
, m_fTempTime(0)
{

}

Kylin::Showgirl::~Showgirl()
{

}

KBOOL Kylin::Showgirl::Initialize()
{
	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(DB_CHARACTER);
	// ��ѯ��Ӧ�Ľ�ɫ��Ϣ
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(m_uGid,dbItem))
		return false;
	
	PropertySet kSet;
	DataItem::DataField dbField;
	dbItem.QueryField("ģ��",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("����",dbField);
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

	kSet.SetValue("$Mesh",sName);
	kSet.SetValue("$Materials",sMaterials);

	if (!Load(kSet))
		return false;
	
	m_pOgreEntity->setQueryFlags(QUERYFLAG_SHOWGIRL);

	return true;
}

KVOID Kylin::Showgirl::Tick( KFLOAT fElapsed )
{

}

KVOID Kylin::Showgirl::Spawn()
{
	

}