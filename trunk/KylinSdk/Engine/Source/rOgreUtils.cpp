#include "engpch.h"
#include "rOgreUtils.h"


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