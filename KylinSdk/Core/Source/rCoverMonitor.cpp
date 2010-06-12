#include "corepch.h"
#include "rCoverMonitor.h"
#include "rOgreUtils.h"
#include "KylinRoot.h"
#include "GameCamera.h"
#include "EntityMaterialInstance.h"


#define MAX_COVER_SPAN 0.2f
#define COVER_OBJ_ALPHA 0.6f

Kylin::PhyX::CoverMonitor::CoverMonitor()
: m_fTimespan(.0f)
{

}

KVOID Kylin::PhyX::CoverMonitor::Tick( KFLOAT fElapsed )
{
	
	if (m_fTimespan > MAX_COVER_SPAN)
	{
		Testing(fElapsed);

		m_fTimespan = .0f;
	}
	
	m_fTimespan += fElapsed;
}

KVOID Kylin::PhyX::CoverMonitor::Destroy()
{
	if ( m_kCoverObj.size() > 0 )
	{
		for ( CoverObjs::iterator it = m_kCoverObj.begin();
			it != m_kCoverObj.end();
			it ++ )
		{
			SAFE_DEL((*it));
		}

		m_kCoverObj.clear();
	}
}

KVOID Kylin::PhyX::CoverMonitor::Testing( KFLOAT fElapsed )
{
	Ogre::Ray kRay		= KylinRoot::GetSingletonPtr()->GetActiveCamera()->GetCameraToTargetRay();
	KFLOAT fDistance	= KylinRoot::GetSingletonPtr()->GetActiveCamera()->GetCameraDistance();
	KVEC<Ogre::Entity *> kEntities;
	if ( OgreUtils::PickEntities(kRay,kEntities,KylinRoot::KR_SCENE_OBJ,fDistance) )
	{
		for (int i = 0; i < kEntities.size(); i++)
		{
			if (!IsInclude(kEntities[i]))
			{
				EntityMaterialInstance* pEnt = KNEW EntityMaterialInstance(kEntities[i]);
				pEnt->setTransparency(COVER_OBJ_ALPHA);
				pEnt->setSceneBlending(SBT_TRANSPARENT_ALPHA);

				m_kCoverObj.push_back(pEnt);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		KBOOL bFlag = false;
		for ( CoverObjs::iterator it = m_kCoverObj.begin();
			it != m_kCoverObj.end();
			it ++ )
		{
			bFlag = false;
			for (int i = 0; i < kEntities.size(); i++)
			{
				if ((*it)->getHost() == kEntities[i])
				{
					bFlag = true;
					break;
				}
			}
			if (bFlag = false)
			{
				SAFE_DEL((*it));
				it = m_kCoverObj.erase(it);
			}
		}
	}
	else
	{
		Destroy();
	}
}

KBOOL Kylin::PhyX::CoverMonitor::IsInclude( Ogre::Entity* pEnt )
{
	for ( CoverObjs::iterator it = m_kCoverObj.begin();
		it != m_kCoverObj.end();
		it ++ )
	{
		if ((*it)->getHost() == pEnt)
		{
			return true;
		}
	}

	return false;
}
