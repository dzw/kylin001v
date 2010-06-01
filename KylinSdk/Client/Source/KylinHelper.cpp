#include "cltpch.h"
#include "KylinHelper.h"
#include "DataManager.h"
#include "DataItem.h"
#include "FileUtils.h"
#include "Property.h"
#include "KylinRoot.h"
#include "DataLoader.h"
#include "rOgreRoot.h"
#include "ScriptVM.h"


Kylin::Entity* Kylin::KylinHelper::SpawnCharactor( KUINT uGid, ClassID uCid )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("CHAR_DB",sValue))
		return NULL;

	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
	// 查询对应的角色信息
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGid,dbItem))
		return false;

	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);
	// 注： 路径前不可有 "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);
	KSTR sDir  = FileUtils::GetRelativePath(sModel);
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists("General",sName))
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");
	
	//////////////////////////////////////////////////////////////////////////
	// addResourceLocation 此方法材质脚步无法解析需要手动加载
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
	
	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFile("./Data/script/charactor/char_1.lua");

	//////////////////////////////////////////////////////////////////////////
	PropertySet kProp;
	kProp.SetValue("$CLASS_ID",(KUINT)uCid);
	kProp.SetValue("$Mesh",sName);
	kProp.SetValue("$Materials",sMaterials);
	kProp.SetValue("$GID",uGid);

	//kProp.SetValue("$CLLSN_SHAPE",(KUINT)1);
	//kProp.SetValue("$CLLSN_TYPE", (KUINT)0);
	//kProp.SetValue("$COLLISION",false);

	Entity * pEnt = KylinRoot::GetSingletonPtr()->SpawnEntity(kProp);
	//////////////////////////////////////////////////////////////////////////

	return pEnt;
}