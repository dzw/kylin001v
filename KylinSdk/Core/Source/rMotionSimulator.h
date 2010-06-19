#pragma once


namespace Kylin
{
	namespace PhyX
	{
		//////////////////////////////////////////////////////////////////////////
		// 运动虚拟体
		class MotionDummy
		{
		public:
			MotionDummy(Node* pHost, KPoint3 kSpeed = KPoint3::ZERO, KFLOAT fGravity = _WGravity)
				: m_pHost(pHost)
				, m_kSpeed(kSpeed)
				, m_bIsInAir(false)
				, m_fGravity(fGravity)
				, m_kPreSpeed(KPoint3::ZERO)
			{
			}
		
		protected:
			//-----------------------------------------------------
			// 落地
			KVOID Touchdown();
			//-----------------------------------------------------
			// 休息状态
			KVOID Repose();
			//-----------------------------------------------------
			// 是否是静止的
			KBOOL IsImmobile();

		protected:
			//-----------------------------------------------------
			// 
			Node*	m_pHost;
			//-----------------------------------------------------
			// 该物体的线性速度
			KPoint3 m_kSpeed;
			// 上一帧物体速度
			KPoint3 m_kPreSpeed;
			// 重力
			KFLOAT	m_fGravity;
			//-----------------------------------------------------
			//此物体是否在空中
			BOOL	m_bIsInAir;

			friend class MotionSimulator;
			friend class Calculator;
		};
		
		//////////////////////////////////////////////////////////////////////////
		// 运动模拟器
		class MotionSimulator
		{
		public:
			MotionSimulator();
						
			virtual KVOID Tick(KFLOAT fElapsed);
			virtual KVOID Destroy();
			//-----------------------------------------------------
			// 提交模拟对象及参数(水平速度，重力加速度)
			virtual KVOID Commit(Node* pNode, const KPoint3 fSpeed, KFLOAT fGravity = _WGravity);
			//-----------------------------------------------------
			// 剔除模拟对象
			virtual KVOID Reject(Node* pNode);
			
		public:
			//-----------------------------------------------------
			// 设置重力加速度
			KVOID SetGravity(KFLOAT fG);

		protected:
			class Calculator
			{
			public:
				Calculator(){}	
				
				KVOID Handle(MotionDummy* pDummy, KFLOAT fElapsed);

			protected:

				friend class MotionSimulator;
			};

		protected:
			typedef KMAP<Node*,MotionDummy*> DummyMap;

			DummyMap	m_kDummyMap;
			Calculator*	m_pCalculator;
		};
	}
}