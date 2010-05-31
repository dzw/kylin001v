#include "corepch.h"
#include "rMotionSimulator.h"
#include "Node.h"
#include "KylinRoot.h"
#include "rOgreUtils.h"
#include "Entity.h"
#include "rOgreRoot.h"


KVOID Kylin::PhyX::MotionDummy::Touchdown()
{
	m_bIsInAir = false;
}

KBOOL Kylin::PhyX::MotionDummy::IsImmobile()
{
	return m_kSpeed == KPoint3::ZERO && m_kPreSpeed == KPoint3::ZERO;
}

#include "ScriptVM.h"
KVOID Kylin::PhyX::MotionDummy::Repose()
{
	//////////////////////////////////////////////////////////////////////////
	// test code
	KUINT uGID = -1;
	Kylin::Entity* pEnt = static_cast<Kylin::Entity*>(m_pHost);
	if ( pEnt->GetPropertyRef().GetUIntValue("$PlayerID",uGID) )
	{
		KSTR sModule = "char_";
		sModule += uGID;

		KVEC<KCCHAR *> kModules;
		kModules.push_back(sModule.data());

		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"do_idle",true,"i",pEnt->GetID());
	}
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

KVOID Kylin::PhyX::MotionSimulator::Commit( Node* pNode, const KPoint3 kSpeed )
{
	assert(pNode);

	DummyMap::iterator it = m_kDummyMap.find(pNode);
	if (it == m_kDummyMap.end())
	{
		MotionDummy* pDummy = KNEW MotionDummy(pNode,kSpeed);
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

KVOID Kylin::PhyX::MotionSimulator::Reject( Node* pNode )
{
	assert(pNode);

	DummyMap::iterator it = m_kDummyMap.find(pNode);
	if (it != m_kDummyMap.end())
	{
		SAFE_DEL(it->second);
		m_kDummyMap.erase(it);
	}
}

KVOID Kylin::PhyX::MotionSimulator::SetGravity( KFLOAT fG )
{
	m_pCalculator->m_fGravity = fG;
}


KVOID Kylin::PhyX::MotionSimulator::Calculator::Handle( MotionDummy* pDummy, KFLOAT fElapsed )
{
	if (pDummy->m_kSpeed == KPoint3::ZERO)
	{
		pDummy->m_kPreSpeed = KPoint3::ZERO;
		pDummy->Repose();
		return;
	}
	// 计算位置
 	KPoint3 kOffset = pDummy->m_kSpeed * fElapsed;
 	kOffset -= m_fGravity * KPoint3::UNIT_Y * fElapsed;
 	pDummy->m_kSpeed -= m_fGravity * KPoint3::UNIT_Y * fElapsed;
	// 设置位置
 	pDummy->m_pHost->GetSceneNode()->translate(kOffset,Ogre::Node::TS_LOCAL);
	
	KPoint3 kCurrPos		= pDummy->m_pHost->GetTranslate();
	KFLOAT	fInAirHeight	= pDummy->m_pHost->GetTranslate().y;
	//-------------------------------------------
	// 获得地面高度
	kCurrPos.y = 50000.0f;
	if (KylinRoot::GetSingletonPtr()->HitTest(kCurrPos,KPoint3(KPoint3::NEGATIVE_UNIT_Y),kCurrPos))
	{
		Ogre::Real r = OgreUtils::GetEntitySize(pDummy->m_pHost->GetEntityPtr(),pDummy->m_pHost->GetScale()).y * KHALF;
		kCurrPos.y += r - KHALF;
	}

	// 跳跃下落状态
	if (pDummy->m_kSpeed.y < 0)
	{
		if( kCurrPos.y >= fInAirHeight )
		{ //已经落地了
			pDummy->m_pHost->SetTranslate(kCurrPos);
			pDummy->Touchdown();
		}
		else
		{
			pDummy->m_bIsInAir = true;
		}
	}
	// 跳跃上升状态
	else if (pDummy->m_kSpeed.y > 0)
	{	// 如果地面高于角色位置
		if( kCurrPos.y >= fInAirHeight )
		{
			pDummy->m_pHost->SetTranslate(kCurrPos);
		}
		
		pDummy->m_bIsInAir = true;
	}
	
	//-------------------------------------------
	if (!pDummy->m_bIsInAir)
	{	// 保存上一帧数据
		pDummy->m_kPreSpeed = pDummy->m_kSpeed;
		pDummy->m_kSpeed = KPoint3::ZERO;
	}
}