#include "corepch.h"
#include "rCollisionMonitor.h"
#include "Node.h"
#include "Entity.h"
#include "KylinRoot.h"
#include "rOgreUtils.h"


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
	m_kSceneVec.clear();
	m_kFloorVec.clear();
}

Kylin::PhyX::CollisionMonitor::CollisionData* Kylin::PhyX::CollisionMonitor::Commit( Node* pHost,KBOOL bCollider,KUINT uSelf,KUINT uMate )
{
	CollisionData* pData = NULL;
	CllsnObjMap::iterator it = m_kObjsMap.find(pHost);
	if (it != m_kObjsMap.end())
	{
		pData = it->second;
		pData->m_bCollider = bCollider;
		pData->m_uSelf = uSelf;
		pData->m_uMate = uMate;
	}
	else
	{
		pData = KNEW CollisionData(pHost,bCollider);
		pData->m_uSelf = uSelf;
		pData->m_uMate = uMate;

		m_kObjsMap.insert(std::pair<Node*, CollisionData*>(pData->m_pHost,pData));
	}

	return pData;
}

KBOOL Kylin::PhyX::CollisionMonitor::QueryScene( KPoint3 kPos, KPoint3 kDir, KFLOAT fRadius)
{
	if (QuerySceneCllsnBox(kPos))
		return false;

	KPoint3 kSrc(kPos.x,kPos.y + 1.5f*fRadius,kPos.z);	
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


KVOID Kylin::PhyX::CollisionMonitor::TestEntities( )
{
	CollisionGroup kGroup;

	for ( CllsnObjMap::iterator it = m_kObjsMap.begin();
		it != m_kObjsMap.end();
		it++ )
	{
		if (it->second->m_bEnable)
		{
			if (it->second->m_bCollider)
			{
				kGroup.AddCollider(it->second);
				kGroup.AddCollidee(it->second);
			}
			else
				kGroup.AddCollidee(it->second);
		}
	}

	kGroup.Update();
}

KBOOL Kylin::PhyX::CollisionMonitor::QuerySceneCllsnBox( KPoint3 kPos )
{
 	for (KUINT i = 0; i < m_kSceneVec.size(); i++)
	{
		if ( m_kSceneVec[i].mOBB.getCenter().squaredDistance(kPos) <= 
			 m_kSceneVec[i].mOBB.getSquaredRadius() )
		{
			if ( m_kSceneVec[i].mOBB.contains(kPos) )
			{
				return true;
			}
		}
	}
	
	return false;
}

KVOID Kylin::PhyX::CollisionMonitor::AddSceneCllsnBox(KCSTR& sName, const OrientedBox& kBox )
{
	CollisionBox box;
	box.mOBB = kBox;
	box.mName = sName;
	m_kSceneVec.push_back(box);
}

KVOID Kylin::PhyX::CollisionMonitor::Erase( Node* pHost )
{
	for ( CllsnObjMap::iterator it = m_kObjsMap.begin();
		it != m_kObjsMap.end();
		it++ )
	{
		if (it->first == pHost)
		{
			SAFE_DEL(it->second);
			m_kObjsMap.erase(it);
			break;
		}
	}
}

KVOID Kylin::PhyX::CollisionMonitor::AddSceneCllsnPlane(KCSTR& sName, const OrientedBox& kBox )
{
	CollisionBox box;
	box.mOBB = kBox;
	box.mName = sName;
	m_kFloorVec.push_back(box);
}

KBOOL Kylin::PhyX::CollisionMonitor::QuerySceneCllsnPlane( KPoint3& kPos , KFLOAT fR)
{
	KFLOAT fDis = .0f;
	for (KUINT i = 0; i < m_kFloorVec.size(); i++)
	{
		if ( m_kFloorVec[i].mOBB.getCenter().squaredDistance(kPos) <= 
			m_kFloorVec[i].mOBB.getSquaredRadius() )
		{
			if ( m_kFloorVec[i].mOBB.contains(kPos) )
			{
				kPos.y = m_kFloorVec[i].mOBB.center.y + m_kFloorVec[i].mOBB.extents.y;

				return true;
			}
		}
	}

	return false;
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
	//m_kCllsnBag.clear();

	KPoint3 kPos1,kPos2;
	for (KUINT i = 0; i < m_kCollider.size(); i++)
	{
		for (KUINT j = 0; j < m_kCollidee.size(); j++)
		{
			if (m_kCollider[i]->m_pHost == m_kCollidee[j]->m_pHost)
				continue;
			
			if (!(m_kCollider[i]->m_uSelf & m_kCollidee[j]->m_uMate))
				continue;

			//----------------------------------------------------------------
			// 过滤掉已经处理过的碰撞对
			if(IsMarked(*m_kCollider[i], *m_kCollidee[j]))
				continue;
			//mark as collided
			AddPair(*m_kCollider[i], *m_kCollidee[j]);
			//----------------------------------------------------------------

			kPos1 = m_kCollider[i]->m_pHost->GetTranslate();
			kPos2 = m_kCollidee[j]->m_pHost->GetTranslate();
			if (kPos1.squaredDistance(kPos2) <= pow(VISIBLE_DISTANCE,2))
			{
				Ogre::AxisAlignedBox& box = m_kCollider[i]->m_pHost->GetWorldBoundingBox();
			
				if (OgreUtils::IntersectsAABB(box,m_kCollidee[j]->m_pHost->GetWorldBoundingBox()) != OgreUtils::OUTSIDE)
				{
					CollisionPair kPair;
					kPair.m_pObj1 = m_kCollider[i]->m_pHost;
					kPair.m_pObj2 = m_kCollidee[j]->m_pHost;

					//m_kCollider[i]->m_pCallback(kPair);
					CollideCallback(kPair);

					if (m_kCollidee[j]->m_eMode == CLLSN_BREAK)
						break;
				}
			}
		}
	}
}


KVOID Kylin::PhyX::CollisionMonitor::CollisionGroup::CollideCallback( const CollisionPair& kPair )
{
	Node* pNode1 = kPair.m_pObj1;
	Node* pNode2 = kPair.m_pObj2;

	Entity* pEnt1 = KylinRoot::GetSingletonPtr()->GetEntity(pNode1->GetWorldID());
	Entity* pEnt2 = KylinRoot::GetSingletonPtr()->GetEntity(pNode2->GetWorldID());

	if (pEnt1 && pEnt2)
	{
		if ( pEnt1->OnShouldCllsn(pEnt2) )
		{
			pEnt1->OnEntityCllsn(pEnt2,KPoint3::ZERO);
		}
	}
}

void Kylin::PhyX::CollisionMonitor::CollisionGroup::AddPair( const CollisionData& kCllsn, const CollisionData& kCllsn_1 )
{
	//add the first of the pair
	ColliderBag::iterator bagIt = m_kCllsnBag.find(&kCllsn);
	if(bagIt != m_kCllsnBag.end())
	{
		bagIt->second.insert(&kCllsn_1);
	}
	else
	{
		ResponserSet cllsnSet;
		cllsnSet.insert(&kCllsn_1);
		m_kCllsnBag.insert(ColliderBag::value_type(&kCllsn, cllsnSet));
	}

	//add the second of the pair
	bagIt = m_kCllsnBag.find(&kCllsn_1);
	if(bagIt != m_kCllsnBag.end())
	{
		bagIt->second.insert(&kCllsn);
	}
	else
	{
		ResponserSet cllsnSet;
		cllsnSet.insert(&kCllsn);
		m_kCllsnBag.insert(ColliderBag::value_type(&kCllsn_1, cllsnSet));
	}	
}

bool Kylin::PhyX::CollisionMonitor::CollisionGroup::IsMarked( const CollisionData& kCllsn, const CollisionData& kCllsn_1 ) const
{
	ColliderBag::const_iterator bagIt = m_kCllsnBag.find(&kCllsn);
	if(bagIt != m_kCllsnBag.end())
	{
		return bagIt->second.find(&kCllsn_1) != bagIt->second.end();
	}
	else
		return false;
}
//////////////////////////////////////////////////////////////////////////

// KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetCallbackFunc( Func pFunc )
// {
// 	assert(pFunc);
// 	m_pCallback = pFunc;
// }

KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetCollsionMode( CllsnMode eMode )
{
	m_eMode = eMode;
}

KVOID Kylin::PhyX::CollisionMonitor::CollisionData::SetEnable( KBOOL bEnable )
{
	m_bEnable = bEnable;
}

