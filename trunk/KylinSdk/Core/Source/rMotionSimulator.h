#pragma once


namespace Kylin
{
	namespace PhyX
	{
		//////////////////////////////////////////////////////////////////////////
		// �˶�������
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
			// ���
			KVOID Touchdown();
			//-----------------------------------------------------
			// ��Ϣ״̬
			KVOID Repose();
			//-----------------------------------------------------
			// �Ƿ��Ǿ�ֹ��
			KBOOL IsImmobile();

		protected:
			//-----------------------------------------------------
			// 
			Node*	m_pHost;
			//-----------------------------------------------------
			// ������������ٶ�
			KPoint3 m_kSpeed;
			// ��һ֡�����ٶ�
			KPoint3 m_kPreSpeed;
			// ����
			KFLOAT	m_fGravity;
			//-----------------------------------------------------
			//�������Ƿ��ڿ���
			BOOL	m_bIsInAir;

			friend class MotionSimulator;
			friend class Calculator;
		};
		
		//////////////////////////////////////////////////////////////////////////
		// �˶�ģ����
		class MotionSimulator
		{
		public:
			MotionSimulator();
						
			virtual KVOID Tick(KFLOAT fElapsed);
			virtual KVOID Destroy();
			//-----------------------------------------------------
			// �ύģ����󼰲���(ˮƽ�ٶȣ��������ٶ�)
			virtual KVOID Commit(Node* pNode, const KPoint3 fSpeed, KFLOAT fGravity = _WGravity);
			//-----------------------------------------------------
			// �޳�ģ�����
			virtual KVOID Erase(Node* pNode);
			
		public:
			//-----------------------------------------------------
			// �����������ٶ�
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