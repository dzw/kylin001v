#include "cltpch.h"
#include "PathwayLoader.h"
#include "StringUtils.h"
#include "XmlStream.h"
#include "Pathway.h"
#include "FileUtils.h"


Kylin::PathwayLoader::PathwayLoader()
{

}

Kylin::PathwayLoader::~PathwayLoader()
{
	KMAP<KUINT,Pathway*>::iterator it = m_kPathwayMap.begin();
	
	for (; it != m_kPathwayMap.end(); it++)
	{
		KDEL it->second;
	}
	
	m_kPathwayMap.clear();
}

KBOOL Kylin::PathwayLoader::Load( KCCHAR* pScene )
{
	Ogre::FileInfoListPtr resPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("General", pScene);
	Ogre::FileInfo fInfo = (*(resPtr->begin()));
	
	KSTR sName = StringUtils::replace(pScene,".xml","_pathway.xml");
	KSTR sPath = fInfo.archive->getName();
	sPath += "/" + sName;
	
	XmlStream kXml(sPath.data());
	if (!kXml.Open(XmlStream::Read))
		return false;

	KBOOL bScene = kXml.SetToFirstChild("pathway");
	while (bScene)
	{
		Pathway* pPathway = KNEW Pathway;

		KUINT id		= kXml.GetAttrInt("id");
		KBOOL bTurnback = kXml.GetAttrBool("turnback");

		KBOOL bPoint = kXml.SetToFirstChild("point");
		while (bPoint)
		{
			KFLOAT fX = kXml.GetAttrFloat("x");
			KFLOAT fZ = kXml.GetAttrFloat("z");
		
			pPathway->Add(KPoint3(fX,0,fZ));

			bPoint = kXml.SetToNextChild("point");
		}

		m_kPathwayMap.insert(std::pair<KUINT,Pathway*>(id,pPathway));

		bScene = kXml.SetToNextChild("pathway");
	}

	kXml.Close();

	return true;
}

Kylin::Pathway* Kylin::PathwayLoader::GetPathway( KUINT uID )
{
	KMAP<KUINT,Pathway*>::iterator it = m_kPathwayMap.find(uID);
	if (it != m_kPathwayMap.end())
	{
		return it->second;
	}

	return NULL;
}

