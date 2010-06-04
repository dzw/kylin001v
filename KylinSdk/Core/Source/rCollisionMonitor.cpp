#include "corepch.h"
#include "rCollisionMonitor.h"
#include "Node.h"
#include "KylinRoot.h"


Kylin::PhyX::CollisionMonitor::CollisionMonitor()
{

}

KVOID Kylin::PhyX::CollisionMonitor::Tick( KFLOAT fElapsed )
{
	TestEntities( );
}

KVOID Kylin::PhyX::CollisionMonitor::Destroy()
{
	for ( CllsnObjMap::iterator it = m_kObjsMap.begin();
		it != m_kObjsMap.end();
		it++ )
	{
		SAFE_DEL(it->second);
	}

	m_kObjsMap.clear();
}

KVOID Kylin::PhyX::CollisionMonitor::Commit( CollisionData* pData )
{
	CllsnObjMap::iterator it = m_kObjsMap.find(pData->m_pHost);
	if (it != m_kObjsMap.end())
	{
		it->second = pData;
	}
	else
	{
		m_kObjsMap.insert(std::pair<Node*, CollisionData*>(pData->m_pHost,pData));
	}
}

KBOOL Kylin::PhyX::CollisionMonitor::QueryScene( KPoint3 kPos, KPoint3 kDir, KFLOAT fRadius )
{
	KPoint3 kSrc(kPos.x,kPos.y + fRadius,kPos.z);	
	Ogre::Ray kRay(kSrc,kDir);
	
	// 检测该射线是否与地面相交
	KPoint3 kRet;
	if (KylinRoot::GetSingletonPtr()->HitTest(kRay,kRet))
	{
		//if (kRet.squaredDistance(kPos) <= fRadius*fRadius)
		{
			kPos = kRet - kPos;
			KFLOAT fDeg = kPos.angleBetween(KPoint3::UNIT_Y).valueDegrees();
			// 如果(p2->p1)与(p2->p0)的倾斜角度小于40，认为无法行走
			// 
			// p1<-------+p0
			//    \       |
			//	   \	  |
			//      \     |
			//		 \	  |
			//		  \   |
			//		   \  |
			//			\ |
			//			 \|
			//			  + p2
			if (fDeg < 40)
				return false;
		}
	}

	return true;
}

Kylin::Node* Kylin::PhyX::CollisionMonitor::Query( const Ogre::Ray& kRay )
{
	
	return NULL;
}

KVOID Kylin::PhyX::CollisionMonitor::TestEntities( )
{
	CollisionGroup kGroup;

	for ( CllsnObjMap::iterator it = m_kObjsMap.begin();
		it != m_kObjsMap.end();
		it++ )
	{
		if (it->second->m_bEnable)
		{
			if (it->second)
				kGroup.AddCollider(it->second);
			else
				kGroup.AddCollidee(it->second);
		}
	}

	kGroup.Update();
}

//////////////////////////////////////////////////////////////////////////
KVOID Kylin::PhyX::CollisionMonitor::CollisionGroup::AddCollider( CollisionData* pData )
{
	m_kCollider.push_back(pData);
}

KVOID Kylin::PhyX::CollisionMonitor::CollisionGroup::AddCollidee( CollisionData* pData )
{
	m_kCollidee.push_back(pData);
}

KVOID Kylin::PhyX::CollisionMonitor::CollisionGroup::Update()
{
	KPoint3 kPos1,kPos2;
	for (KUINT i = 0; i < m_kCollider.size(); i++)
	{
		for (KUINT j = 0; j < m_kCollidee.size(); j++)
		{
			kPos1 = m_kCollider[i]->m_pHost->GetTranslate();
			kPos2 = m_kCollidee[j]->m_pHost->GetTranslate();
			if (kPos1.squaredDistance(kPos2) <= pow((m_kCollider[i]->m_fRadius + m_kCollidee[j]->m_fRadius),2))
			{
				CollisionPair kPair;
				kPair.m_pObj1 = m_kCollider[i]->m_pHost;
				kPair.m_pObj2 = m_kCollidee[j]->m_pHost;
				
				m_kCollider[i]->m_pCallback(kPair);

				if (m_kCollidee[j]->m_eMode == CLLSN_BREAK)
					break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////

KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetCallbackFunc( Func pFunc )
{
	assert(pFunc);
	m_pCallback = pFunc;
}

KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetCollsionMode( CllsnMode eMode )
{
	m_eMode = eMode;
}

KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetEnable( KBOOL bEnable )
{
	m_bEnable = bEnable;
}