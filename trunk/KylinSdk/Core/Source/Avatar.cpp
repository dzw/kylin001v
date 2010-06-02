#include "corepch.h"
#include "Avatar.h"



KVOID Kylin::Avatar::Exchange( Node* pNode, KUINT uGID )
{

}

KVOID Kylin::Avatar::Exchange( Ogre::Entity* pHost, AvatarPart eType, KSTR sMat )
{
	SubEntity* pSub = NULL;
	for(int i = 0 ; i < pHost->getNumSubEntities(); i++)
	{
		pSub = pHost->getSubEntity(i);
		if (pSub)
		{		
			String sName = pSub->getSubMesh()->getMaterialName();
			if (eType == AP_CHEST && sName.find("/chest") != -1)
				goto FLAG_T;
			else if (eType == AP_HELMET && sName.find("/helmet") != -1)
				goto FLAG_T;
			else if (eType == AP_GLOVES && sName.find("/gloves") != -1)
				goto FLAG_T;
			else if (eType == AP_BOOTS && sName.find("/boots") != -1)
				goto FLAG_T;
			else if (eType == AP_SHOULDERS && sName.find("/shoulders") != -1)
				goto FLAG_T;
			else if (eType == AP_FACE && sName.find("/face") != -1)
				goto FLAG_T;
		}
	}
	
	goto FLAG_F;

FLAG_T:
	//
	if (pSub)
	{
		Ogre::MaterialPtr matEnt = Ogre::MaterialManager::getSingletonPtr()->getByName(sMat);
		pSub->setMaterial(matEnt);
	}

FLAG_F:
	assert(!"装备类型错误或此模型不可以换装！");
}