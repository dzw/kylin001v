#include "engpch.h"
#include "AnimationProxy.h"


Kylin::AnimationProxy::AnimationProxy( Ogre::Entity* pHost )
: m_pEntity(pHost)
, m_pSource(NULL)
, m_pTarget(NULL)
{
	SetTarget(pHost);
}

Kylin::AnimationProxy::AnimationProxy()
: m_pEntity(NULL)
, m_pSource(NULL)
, m_pTarget(NULL)
{

}

Kylin::AnimationProxy::~AnimationProxy()
{

}

KVOID Kylin::AnimationProxy::Play( KSTR sAnim, KBOOL bL /*= false*/, BlendingTransition eTransition /*= BlendSwitch*/, KFLOAT fDuration /*= 1.0*/ )
{
	if (!m_pEntity->getAnimationState( sAnim ))
	{
		AssertEx(NULL,KSTR("不存在此动画： " + sAnim).data());
		return;
	}

	m_bLoop = bL; //设置是否需要循环
	if( eTransition == AnimationProxy::BlendSwitch )
	{	//如果混合方式为直接切换，改变m_pSource 即可
		if( m_pSource != 0 )
			m_pSource->setEnabled(false);
		m_pSource = m_pEntity->getAnimationState( sAnim );
		m_pSource->setEnabled(true);
		m_pSource->setWeight(1);
		m_pSource->setTimePosition(0);
		m_fTimeleft = 0;
	} 
	else 
	{ 
		//先取得新的动画状态
		AnimationState *newTarget = m_pEntity->getAnimationState( sAnim );
		if( m_fTimeleft > 0 ) //前一次的混合尚未结束
		{
			if( newTarget == m_pTarget )
			{
				// 新的目标就是正在混合中的目标，什么也不做
			}
			else if( newTarget == m_pSource )
			{
				// 新的目标是源动画，直接go back
				m_pSource = m_pTarget;
				m_pTarget = newTarget;
				m_fTimeleft = m_fDuration - m_fTimeleft; 
			}
			else
			{
				// 现在newTarget是真的新的动画了
				if( m_fTimeleft < m_fDuration * 0.5 ) //上一次的混合进度还未超过一半
				{
					// 简单替换Target就行了
					m_pTarget->setEnabled(false);
					m_pTarget->setWeight(0);
				}
				else //如果已经过半，旧的target成为新的source
				{
					m_pSource->setEnabled(false);
					m_pSource->setWeight(0);
					m_pSource = m_pTarget;
				} 
				m_pTarget = newTarget;
				m_pTarget->setEnabled(true);
				m_pTarget->setWeight( 1.0 - m_fTimeleft / m_fDuration );
				m_pTarget->setTimePosition(0);
			}
		}
		else //上次的混合已经结束，当前未处于混合状态中
		{
			m_eTransition = eTransition;
			m_fTimeleft = m_fDuration = fDuration;
			m_pTarget = newTarget;
			m_pTarget->setEnabled(true);
			m_pTarget->setWeight(0);
			m_pTarget->setTimePosition(0);
		}
	}
}

KVOID Kylin::AnimationProxy::Play( KINT nIndex, KBOOL bL /*= false*/, BlendingTransition eTransition /*= BlendSwitch*/, KFLOAT fDuration /*= 1.0*/ )
{
	if (nIndex >= m_kAnimNameList.size())
		return;

	KSTR sAnim = m_kAnimNameList[nIndex];
	if(!sAnim.empty())
	{
		Play(sAnim,bL,eTransition,fDuration);
	}
}


KVOID Kylin::AnimationProxy::Update( KFLOAT fElapsed )
{
	if( m_pSource != 0 )
	{
		if( m_fTimeleft > 0 ) //两个动画仍在混合过程中
		{
			m_fTimeleft -= fElapsed; 
			if( m_fTimeleft < 0 )
			{
				// 混合完毕,切换到目标动画
				m_pSource->setEnabled(false);
				m_pSource->setWeight(0);
				m_pSource = m_pTarget;
				m_pSource->setEnabled(true);
				m_pSource->setWeight(1);
				m_pTarget = 0;
			}
			else
			{
				// 仍然处于混合状态中，改变两个动画的权值
				m_pSource->setWeight(m_fTimeleft / m_fDuration);
				m_pTarget->setWeight(1.0 - m_fTimeleft / m_fDuration);
				//在这种混合方式下，需要为目标动画增加时间
				if(m_eTransition == AnimationProxy::BlendWhileAnimating)
					m_pTarget->addTime(fElapsed);
			}
		}
		if (m_pSource->getTimePosition() >= m_pSource->getLength())
		{
			// 动画结束			
		}

		m_pSource->addTime(fElapsed);
		m_pSource->setLoop(m_bLoop);
	}
}

KVOID Kylin::AnimationProxy::Reset()
{
	m_fTimeleft = 0;
	m_fDuration = 1;
	m_pTarget	= NULL;
}

KVOID Kylin::AnimationProxy::SetTarget( Ogre::Entity * pEnt )
{
	m_pEntity = pEnt;
	if (m_pEntity)
	{
		m_kAnimNameList.clear();
		//初始化所有动作的AnimationState
		AnimationStateSet *set = m_pEntity->getAllAnimationStates();
		if (set)
		{
			AnimationStateIterator it = set->getAnimationStateIterator();
			while(it.hasMoreElements())
			{
				AnimationState *pAnim = it.getNext();
				pAnim->setEnabled(false);
				pAnim->setWeight(0);
				pAnim->setTimePosition(0);
				m_kAnimNameList.push_back(pAnim->getAnimationName());
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	Reset();
}

KFLOAT Kylin::AnimationProxy::GetLength( KSTR sAnim )
{
	Ogre::AnimationState *pSource = m_pEntity->getAnimationState( sAnim );
	if (pSource)
		return pSource->getLength();

	return .0f;
}
