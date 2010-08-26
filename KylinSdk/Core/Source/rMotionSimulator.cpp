#include "corepch.h"
#include "rMotionSimulator.h"
#include "Node.h"
#include "KylinRoot.h"
#include "rOgreUtils.h"
#include "Entity.h"
#include "rOgreRoot.h"
#include "rPhyxSystem.h"
#include "rCollisionMonitor.h"
#include "ScriptVM.h"
#include "RemoteEvents.h"
#include "ScriptFunctions.h"


KVOID Kylin::PhyX::MotionDummy::Touchdown()
{
	m_bIsInAir = false;
	if (m_pHost)
	{
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_pHost->GetWorldID());
		if (pEnt)
		{
			//////////////////////////////////////////////////////////////////////////
			EventPtr spEV(
				KNEW Event(
				&ev_post_touchdown, 
				Event::ev_immediate, 
				0, 
				0, 
				NULL
				));

			KylinRoot::GetSingletonPtr()->PostMessage(pEnt->GetID(),spEV);
		}
	}
}

KBOOL Kylin::PhyX::MotionDummy::IsImmobile()
{
	return m_kSpeed == KPoint3::ZERO && m_kPreSpeed == KPoint3::ZERO && !m_bIsInAir;
}


KVOID Kylin::PhyX::MotionDummy::Repose()
{
	//////////////////////////////////////////////////////////////////////////
	Script::to_stop(m_pHost->GetWorldID());
}

Kylin::PhyX::MotionSimulator::MotionSimulator()
{
	m_pCalculator = KNEW Calculator();

}

KVOID Kylin::PhyX::MotionSimulator::Tick( KFLOAT fElapsed )
{
	for ( DummyMap::iterator it = m_kDummyMap.begin();
		  it != m_kDummyMap.end();
		  it++ )
	{
		if (!it->second->IsImmobile())
			SAFE_CALL(m_pCalculator,Handle(it->second, fElapsed));
	}
}

KVOID Kylin::PhyX::MotionSimulator::Destroy()
{
	SAFE_DEL(m_pCalculator);

	for ( DummyMap::iterator it = m_kDummyMap.begin();
		it != m_kDummyMap.end();
		it++ )
	{
		SAFE_DEL(it->second);
	}

	m_kDummyMap.clear();
}

KVOID Kylin::PhyX::MotionSimulator::Commit( Node* pNode, const KPoint3 kSpeed, KFLOAT fGravity)
{
	assert(pNode);

	DummyMap::iterator it = m_kDummyMap.find(pNode);
	if (it == m_kDummyMap.end())
	{
		MotionDummy* pDummy = KNEW MotionDummy(pNode,kSpeed,fGravity);
		m_kDummyMap.insert(std::pair<Node*,MotionDummy*>(pNode,pDummy));		
	}
	else
	{
		MotionDummy* pDummy = it->second;

		if (!pDummy->m_bIsInAir)
		{
			if (kSpeed.y > 0)
			{
				pDummy->m_kSpeed.z = pDummy->m_kPreSpeed.z;
				pDummy->m_kSpeed.y = kSpeed.y;
			}
			else
				pDummy->m_kSpeed = kSpeed;
		}
	}
}

KVOID Kylin::PhyX::MotionSimulator::Erase( Node* pNode )
{
	assert(pNode);

	DummyMap::iterator it = m_kDummyMap.find(pNode);
	if (it != m_kDummyMap.end())
	{
		SAFE_DEL(it->second);
		m_kDummyMap.erase(it);
	}
}


KVOID Kylin::PhyX::MotionSimulator::Calculator::Handle( MotionDummy* pDummy, KFLOAT fElapsed )
{
	if ( pDummy->m_kPreSpeed != KPoint3::ZERO && 
		 pDummy->m_kSpeed == KPoint3::ZERO && 
		 !pDummy->m_bIsInAir )
	{
		pDummy->m_kPreSpeed = KPoint3::ZERO;
		pDummy->Repose();
		return;
	}

	// ����λ��
 	KPoint3 kOffset = pDummy->m_kSpeed * fElapsed;
 	kOffset -= pDummy->m_fGravity * KPoint3::UNIT_Y * fElapsed;
 	pDummy->m_kSpeed -= pDummy->m_fGravity * KPoint3::UNIT_Y * fElapsed;

	//---------------------------------------------------------------
	// ����λ�ã�������λ��
	KPoint3 kOldPos		= pDummy->m_pHost->GetTranslate();
 	pDummy->m_pHost->GetSceneNode()->translate(kOffset,Ogre::Node::TS_LOCAL);
	KPoint3 kCurrPos	= pDummy->m_pHost->GetTranslate();
	
	//---------------------------------------------------------------
	// ���ʵ���С
	KFLOAT r	= OgreUtils::GetEntitySize(pDummy->m_pHost->GetEntityPtr(),pDummy->m_pHost->GetScale()).y * KHALF;
	KPoint3 dir = pDummy->m_pHost->GetRotation() * Ogre::Vector3::UNIT_Z;

	// ���㳡����ײ
	if (!PhysicalSystem::GetSingletonPtr()->GetCollisionMonitor()->QueryScene(kCurrPos,dir,r))
	{	// �������ϰ����ó�ԭλ��
		pDummy->m_kSpeed = KPoint3::ZERO;
		pDummy->m_pHost->SetTranslate(kOldPos);
	}
	
	KFLOAT	fInAirHeight = kCurrPos.y;
	if (!PhysicalSystem::GetSingletonPtr()->GetCollisionMonitor()->QuerySceneCllsnPlane(kCurrPos,r))
	{
		//---------------------------------------------------------------
		// ��õ���߶�
		kCurrPos.y = 50000.0f;
		KylinRoot::GetSingletonPtr()->GetTerrainHeight(kCurrPos);//HitTest(kCurrPos,KPoint3(KPoint3::NEGATIVE_UNIT_Y),kCurrPos);
	}
	
	// ��Ծ����״̬
	if (pDummy->m_kSpeed.y < 0)
	{
		if( kCurrPos.y >= fInAirHeight )
		{ //�Ѿ����
			pDummy->m_pHost->SetTranslate(kCurrPos);
			pDummy->Touchdown();
		}
		else
		{
			pDummy->m_bIsInAir = true;
		}
	}
	// ��Ծ����״̬
	else if (pDummy->m_kSpeed.y > 0)
	{	// ���������ڽ�ɫλ��
		if( kCurrPos.y >= fInAirHeight )
		{
			pDummy->m_pHost->SetTranslate(kCurrPos);
		}
		
		pDummy->m_bIsInAir = true;
	}
	
	//---------------------------------------------------------------
	if (!pDummy->m_bIsInAir)
	{	// ������һ֡����
		pDummy->m_kPreSpeed = pDummy->m_kSpeed;
		pDummy->m_kSpeed = KPoint3::ZERO;
	}
}