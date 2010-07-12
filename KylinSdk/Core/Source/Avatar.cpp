#include "corepch.h"
#include "Avatar.h"
#include "Node.h"
#include "DataItem.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "Character.h"
#include "FileUtils.h"
#include "rOgreUtils.h"


Kylin::Avatar::Avatar(Character* pChar)
: m_pHost(pChar)
, m_pLWeapon(NULL)
, m_pRWeapon(NULL)
{

}

Kylin::Avatar::~Avatar()
{
	DetachWeapon(AP_RWEAPON);
	DetachWeapon(AP_LWEAPON);
}

KVOID Kylin::Avatar::Exchange( KUINT uGID )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("AVATAR_DB",sValue))
		return;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// ��ѯ��Ӧ��Avatar��Ϣ
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGID,dbItem))
		return;

	DataItem::DataField dbField;
	dbItem.QueryField("TYPE",dbField);
	KSTR sType = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);
	
	AvatarPart eType;
	if (sType == "chest")
		eType = AP_CHEST;
	else if (sType == "helmet")
		eType = AP_HELMET;
	else if (sType == "shoulders")
		eType = AP_SHOULDERS;
	else if (sType == "face")
		eType = AP_FACE;
	else if (sType == "gloves")
		eType = AP_GLOVES;
	else if (sType == "boots")
		eType = AP_BOOTS;
	else
	{
		AssertEx(NULL,"װ�����ʹ���");
		return;
	}

	Exchange(m_pHost->GetEntityPtr(), eType, sMaterials);

}

KVOID Kylin::Avatar::Exchange( Ogre::Entity* pHost, AvatarPart eType, KSTR sMat )
{
	Ogre::SubEntity* pSub = NULL;
	for(int i = 0 ; i < pHost->getNumSubEntities(); i++)
	{
		pSub = pHost->getSubEntity(i);
		if (pSub)
		{		
			Ogre::String sName = pSub->getSubMesh()->getMaterialName();
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
	AssertEx(NULL,"װ�����ʹ�����ģ�Ͳ����Ի�װ��");
}

//////////////////////////////////////////////////////////////////////////
Kylin::Node* Kylin::Avatar::AttachWeapon( KUINT uGID, AvatarPart ePart /*= AP_RWEAPON*/ )
{
	DetachWeapon(ePart);

	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("AVATAR_DB",sValue))
		return NULL;

	DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);

	// ��ѯ��Ӧ��Avatar��Ϣ
	DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGID,dbItem))
		return NULL;

	DataItem::DataField dbField;
	dbItem.QueryField("TYPE",dbField);
	KSTR sType = boost::any_cast<KSTR>(dbField.m_aValue);
	if (sType != "weapon")
		return NULL;

	dbItem.QueryField("MESH",dbField);
	KSTR sMesh = boost::any_cast<KSTR>(dbField.m_aValue);
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);

	// ע�� ·��ǰ������ "\"
	if (!FileUtils::IsFileExist(sMesh))
		return NULL;

	OgreUtils::DynamicLoadMesh(sMesh);

	//////////////////////////////////////////////////////////////////////////
	KSTR sName = FileUtils::GetFileNameWithSuffix(sMesh);

	PropertySet kProp;
	kProp.SetValue("$Mesh",sName);
	//kProp.SetValue("$Materials",sMaterials);
	kProp.SetValue("$GID",uGID);
	//kProp.SetValue("$Shadows",true);

	Node* pWeapon = KNEW Node();

	if ( !pWeapon->Load(kProp) )
	{
		SAFE_DEL(pWeapon);
		return NULL;
	}

	if (ePart == AP_RWEAPON)
	{
		m_pRWeapon = pWeapon;
		m_pHost->AttachMesh(pWeapon->GetEntityPtr(),"tag_righthand");
	}
	else
	{
		m_pLWeapon = pWeapon;
		m_pHost->AttachMesh(pWeapon->GetEntityPtr(),"tag_lefthand");
	}

	return pWeapon;
}

KVOID Kylin::Avatar::DetachWeapon( AvatarPart ePart )
{
	if (ePart == AP_RWEAPON)
	{
		if (m_pRWeapon)
		{
			m_pHost->DetachMesh(m_pRWeapon->GetEntityPtr());
			SAFE_DEL(m_pRWeapon);
		}
	}
	else
	{
		if (m_pLWeapon)
		{
			m_pHost->DetachMesh(m_pLWeapon->GetEntityPtr());
			SAFE_DEL(m_pLWeapon);
		}
	}
}
