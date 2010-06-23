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

KVOID Kylin::AnimationProxy::Play( KCSTR& sAnim, KBOOL bL /*= false*/, BlendingTransition eTransition /*= BlendSwitch*/, KFLOAT fDuration /*= 1.0*/ )
{
	if (!HasAnimation( sAnim ))
	{
		//AssertEx(NULL,KSTR("�����ڴ˶����� " + sAnim).data());
		return;
	}

	m_bLoop = bL; //�����Ƿ���Ҫѭ��
	if( eTransition == AnimationProxy::BlendSwitch )
	{	//�����Ϸ�ʽΪֱ���л����ı�m_pSource ����
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
		//��ȡ���µĶ���״̬
		AnimationState *newTarget = m_pEntity->getAnimationState( sAnim );
		if( m_fTimeleft > 0 ) //ǰһ�εĻ����δ����
		{
			if( newTarget == m_pTarget )
			{
				// �µ�Ŀ��������ڻ���е�Ŀ�꣬ʲôҲ����
			}
			else if( newTarget == m_pSource )
			{
				// �µ�Ŀ����Դ������ֱ��go back
				m_pSource = m_pTarget;
				m_pTarget = newTarget;
				m_fTimeleft = m_fDuration - m_fTimeleft; 
			}
			else
			{
				// ����newTarget������µĶ�����
				if( m_fTimeleft < m_fDuration * 0.5 ) //��һ�εĻ�Ͻ��Ȼ�δ����һ��
				{
					// ���滻Target������
					m_pTarget->setEnabled(false);
					m_pTarget->setWeight(0);
				}
				else //����Ѿ����룬�ɵ�target��Ϊ�µ�source
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
		else //�ϴεĻ���Ѿ���������ǰδ���ڻ��״̬��
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
	if( m_pSource )
	{
		if( m_fTimeleft > 0 ) //�����������ڻ�Ϲ�����
		{
			m_fTimeleft -= fElapsed; 
			if( m_fTimeleft < 0 )
			{
				// ������,�л���Ŀ�궯��
				m_pSource->setEnabled(false);
				m_pSource->setWeight(0);
				m_pSource = m_pTarget;
				m_pSource->setEnabled(true);
				m_pSource->setWeight(1);
				m_pTarget = 0;
			}
			else
			{
				// ��Ȼ���ڻ��״̬�У��ı�����������Ȩֵ
				m_pSource->setWeight(m_fTimeleft / m_fDuration);
				m_pTarget->setWeight(1.0 - m_fTimeleft / m_fDuration);
				//�����ֻ�Ϸ�ʽ�£���ҪΪĿ�궯������ʱ��
				if(m_eTransition == AnimationProxy::BlendWhileAnimating)
					m_pTarget->addTime(fElapsed);
			}
		}
		if (m_pSource->getTimePosition() >= m_pSource->getLength())
		{
			// ��������
			PlayNext();
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
		//��ʼ�����ж�����AnimationState
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

KFLOAT Kylin::AnimationProxy::GetLength( KCSTR& sAnim )
{
	Ogre::AnimationState *pSource = m_pEntity->getAnimationState( sAnim );
	if (pSource)
		return pSource->getLength();

	return .0f;
}

KBOOL Kylin::AnimationProxy::HasAnimation( KCSTR& sAnim )
{
	for (KUINT i = 0 ; i < m_kAnimNameList.size(); i++)
	{
		if (sAnim == m_kAnimNameList[i])
		{
			return true;
		}
	}

	return false;
}

KVOID Kylin::AnimationProxy::AddQueue( KCSTR& sAnim )
{
	if (HasAnimation(sAnim))
	{
		if (m_pSource && m_pSource->getTimePosition() < m_pSource->getLength())
		{
			m_kWaitingQueue.push_back(sAnim);
		}
		
	}
}

KBOOL Kylin::AnimationProxy::IsEmptyQueue()
{
	return !m_kWaitingQueue.size();
}

KVOID Kylin::AnimationProxy::PlayNext()
{
	if (!IsEmptyQueue())
	{
		Play(m_kWaitingQueue[0]);
		m_kWaitingQueue.erase(m_kWaitingQueue.begin());
	}
}

KVOID Kylin::AnimationProxy::ClearQueue()
{
	m_kWaitingQueue.clear();
}