#pragma once


namespace Kylin
{
	class AnimationProxy
	{
	public:
		AnimationProxy();
		AnimationProxy(Ogre::Entity* pHost);
		virtual ~AnimationProxy();

		enum BlendingTransition		// 不同的混合方式
		{
			BlendSwitch,			// 直接切换到目标动画
			BlendWhileAnimating,	// 交叉淡入淡出（源动画比例缩小，同时目标动画比例增大）
			BlendThenAnimate		// 淡出源动画到目标动画第一帧，然后开始目标动画 
		};
		
		// 播放动画
		KVOID Play(KCSTR& sAnim, KBOOL bL = false, BlendingTransition eTransition = BlendSwitch, KFLOAT fDuration = 1.0);
		KVOID Play(KINT nIndex, KBOOL bL = false, BlendingTransition eTransition = BlendSwitch, KFLOAT fDuration = 1.0);
		KVOID Update(KFLOAT fElapsed);
		KVOID Reset();

		KVOID SetTarget(Ogre::Entity *);
		// 获得动画长度
		KFLOAT GetLength(KCSTR& sAnim);
		
		KBOOL IsPlaying(KCSTR& sAnim);

		KINT HasAnimation(KCSTR& sAnim);
		
		// 将动画加入等待队列，上一个动画播放完后再播放此动画
		KVOID AddQueue(KCSTR& sAnim);
		// 判断队列是否为空
		KBOOL IsEmptyQueue();
		// 播放队列中的下一个动画
		KVOID PlayNext();
		// 清空队列
		KVOID ClearQueue();
		
		// 设置回调对象
		virtual KVOID SetCallbackObj(ClockingCallback* pObj);
		// 设置用户数据
		virtual KVOID SetUserData(KANY aData) { m_kUserData = aData; } 

	protected:
		Ogre::Entity *			m_pEntity;
		Ogre::AnimationState *	m_pSource;
		Ogre::AnimationState *	m_pTarget;

		ClockingCallback*		m_pClocking;	// 定时回调
		KANY					m_kUserData;

		BlendingTransition		m_eTransition;

		KBOOL					m_bLoop;		// 是否循环
		KFLOAT					m_fTimeleft;
		KFLOAT					m_fDuration;	// 持续时间

		KVEC<KSTR>				m_kAnimNameList;// 动画名称列表

		KVEC<KSTR>				m_kWaitingQueue;// 动画等待队列
	};
}