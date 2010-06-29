#pragma once


enum AI_STATE
{
	AS_INVALID	= -1,
	AS_IDLE,				//������
	AS_MOVE,				//�ƶ���
	AS_JUMP,
	AS_USE_SKILL,			//����ʹ����
	AS_DEAD,				//����
	AS_FOLLOW,				//����
	
	AS_NUMBERS
};


// Run Command �ķ���ֵ
enum RC_RESULT
{
	RC_OK = 0,
	RC_ERROR,
	RC_SKIP,
	RC_WAIT, // �ȴ�
};

class CRandomMersenne;
namespace Kylin
{
	typedef std::vector<KPoint3> Pathway;

	class BaseAI
	{
	public:
		BaseAI(Character* pHost);
		~BaseAI();

		virtual KBOOL Init();
		virtual KVOID Tick(KFLOAT fElapsed);
		
		virtual KVOID Reset();

	public:
		KVOID		SetPathway(const Pathway& kPaths);
		KVOID		AddPathwayPos(const KPoint3& kPos);

	public:
		//-------------------------------------
		//����ĳ�߼�״̬
		virtual RC_RESULT	Enter_Idle( KVOID );
		virtual RC_RESULT	Enter_Jump( KVOID );
		virtual RC_RESULT	Enter_Move( FLOAT fDestX, FLOAT fDestZ );
		virtual RC_RESULT	Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KFLOAT fDir );
		virtual RC_RESULT	Enter_Dead( KVOID );
		virtual RC_RESULT	Enter_Follow( KUINT uTargetObj );
		virtual RC_RESULT	Enter_Patrol();

	protected:
		//-------------------------------------
		//ĳ�߼�״̬�е�Tick��
		virtual KBOOL		Tick_Idle( KFLOAT fElapsed );
		virtual KBOOL		Tick_Move( KFLOAT fElapsed );
		virtual KBOOL		Tick_Jump( KFLOAT fElapsed );
		virtual KBOOL		Tick_UseSkill( KFLOAT fElapsed );
		virtual KBOOL		Tick_Dead( KFLOAT fElapsed );
		virtual KBOOL		Tick_Follow( KFLOAT fElapsed );

	protected:
		Character*	m_pHostChar;
		AI_STATE	m_eCurrState;
		
		KPoint3		m_kDestination;				// Ŀ�ĵ�λ��
		KFLOAT		m_fDistance;				// ��Ŀ���ľ���

		Pathway		m_kPathway;					// ·��
		KINT		m_nPathwayIndex;			// ��ǰ·��������
		
		KFLOAT		m_fStayTime;				// ͣ��ʱ��

		CRandomMersenne* m_pRandomGenerator;	// �����������
	};
}

