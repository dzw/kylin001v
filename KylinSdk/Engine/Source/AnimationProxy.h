#pragma once


namespace Kylin
{
	class AnimationProxy
	{
	public:
		AnimationProxy();
		AnimationProxy(Ogre::Entity* pHost);
		virtual ~AnimationProxy();

		enum BlendingTransition		// ��ͬ�Ļ�Ϸ�ʽ
		{
			BlendSwitch,			// ֱ���л���Ŀ�궯��
			BlendWhileAnimating,	// ���浭�뵭����Դ����������С��ͬʱĿ�궯����������
			BlendThenAnimate		// ����Դ������Ŀ�궯����һ֡��Ȼ��ʼĿ�궯�� 
		};
		
		// ���Ŷ���
		KVOID Play(KCSTR& sAnim, KBOOL bL = false, BlendingTransition eTransition = BlendSwitch, KFLOAT fDuration = 1.0);
		KVOID Play(KINT nIndex, KBOOL bL = false, BlendingTransition eTransition = BlendSwitch, KFLOAT fDuration = 1.0);
		KVOID Update(KFLOAT fElapsed);
		KVOID Reset();

		KVOID SetTarget(Ogre::Entity *);
		// ��ö�������
		KFLOAT GetLength(KCSTR& sAnim);
		
		KBOOL HasAnimation(KCSTR& sAnim);
		
		// ����������ȴ����У���һ��������������ٲ��Ŵ˶���
		KVOID AddQueue(KCSTR& sAnim);
		// �ж϶����Ƿ�Ϊ��
		KBOOL IsEmptyQueue();
		// ���Ŷ����е���һ������
		KVOID PlayNext();
		// ��ն���
		KVOID ClearQueue();
		
		// ���ûص�����
		virtual KVOID SetCallbackObj(ClockingCallback* pObj);
		// �����û�����
		virtual KVOID SetUserData(KANY aData) { m_kUserData = aData; } 

	protected:
		Ogre::Entity *			m_pEntity;
		Ogre::AnimationState *	m_pSource;
		Ogre::AnimationState *	m_pTarget;

		ClockingCallback*		m_pClocking;	// ��ʱ�ص�
		KANY					m_kUserData;

		BlendingTransition		m_eTransition;

		KBOOL					m_bLoop;		// �Ƿ�ѭ��
		KFLOAT					m_fTimeleft;
		KFLOAT					m_fDuration;	// ����ʱ��

		KVEC<KSTR>				m_kAnimNameList;// ���������б�

		KVEC<KSTR>				m_kWaitingQueue;// �����ȴ�����
	};
}