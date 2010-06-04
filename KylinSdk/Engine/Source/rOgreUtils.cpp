#include "engpch.h"
#include "rOgreUtils.h"
#include "FileUtils.h"


KPoint3 Kylin::OgreUtils::GetEntitySize( Ogre::MovableObject* pObj, KFLOAT fScale )
{
	//get mesh size  from bouding box : 
	Ogre::AxisAlignedBox spbox = pObj->getBoundingBox();      
	KPoint3 min = spbox.getMinimum()*fScale;
	KPoint3 max = spbox.getMaximum()*fScale;
	KFLOAT	paddingFactor = Ogre::MeshManager::getSingleton().getBoundsPaddingFactor();
	KPoint3 newMin = min+(max-min)*paddingFactor;
	KPoint3 newMax = max+(min-max)*paddingFactor;
	return	newMax - newMin;
}

KVOID Kylin::OgreUtils::DynamicLoadMesh( KSTR sMesh )
{
	KSTR sName = FileUtils::GetFileNameWithSuffix(sMesh);
	KSTR sDir  = FileUtils::GetRelativePath(sMesh);
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists("General",sName))
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");

	//////////////////////////////////////////////////////////////////////////
	// addResourceLocation 此方法材质脚步无法解析需要手动加载
	KSTR sMat = FileUtils::GetFileName(sMesh);
	sMat += ".material";
	if (!Ogre::MaterialManager::getSingletonPtr()->resourceExists(sMat))
	{
		sMat = sDir + sMat;
		std::ifstream fs(sMat.data());
		Ogre::DataStreamPtr stream = Ogre::DataStreamPtr(OGRE_NEW Ogre::FileStreamDataStream(&fs, false));
		Ogre::MaterialManager::getSingletonPtr()->parseScript(stream,"General");
	}
	//////////////////////////////////////////////////////////////////////////
}