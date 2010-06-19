#pragma once


enum AI_STATE
{
	AS_INVALID	= -1,
	AS_IDLE,				//空闲中
	AS_MOVE,				//移动中
	AS_JUMP,
	AS_USE_SKILL,			//技能使用中
	AS_DEAD,				//死亡
	AS_FOLLOW,				//跟随
	
	AS_NUMBERS
};


// Run Command 的返回值
enum RC_RESULT
{
	RC_OK = 0,
	RC_ERROR,
	RC_SKIP,
	RC_WAIT, // 等待
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
		//进入某逻辑状态
		RC_RESULT	Enter_Idle( KVOID );
		RC_RESULT	Enter_Jump( KVOID );
		RC_RESULT	Enter_Move( FLOAT fDestX, FLOAT fDestZ );
		RC_RESULT	Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KFLOAT fDir );
		RC_RESULT	Enter_Dead( KVOID );
		RC_RESULT	Enter_Follow( KUINT uTargetObj );
		RC_RESULT	Enter_Patrol();

	protected:
		//-------------------------------------
		//某逻辑状态中的Tick桢
		KBOOL		Tick_Idle( KFLOAT fElapsed );
		KBOOL		Tick_Move( KFLOAT fElapsed );
		KBOOL		Tick_Jump( KFLOAT fElapsed );
		KBOOL		Tick_UseSkill( KFLOAT fElapsed );
		KBOOL		Tick_Dead( KFLOAT fElapsed );
		KBOOL		Tick_Follow( KFLOAT fElapsed );

	protected:
		Character*	m_pHostChar;
		AI_STATE	m_eCurrState;

		KPoint3		m_kDestination;			// 目的地位置
		KFLOAT		m_fDistance;			// 离目标点的距离

		Pathway		m_kPathway;				// 路径
		KINT		m_nPathwayIndex;		// 当前路径点索引
		
		KFLOAT		m_fStayTime;			// 停留时间

		CRandomMersenne* m_pRandomGenerator;	// 随机数发生器
	};
}

