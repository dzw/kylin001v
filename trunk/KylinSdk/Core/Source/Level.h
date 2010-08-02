#pragma once

#include "Entity.h"
#include "core.h"

namespace Kylin
{
	
	class Level : public Entity
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(Level, Entity)
		Declare_Entity_Creator(Level)

		Level(KVOID);
		virtual ~Level(KVOID);
			
		virtual	KBOOL	Init(const PropertySet& kProp);
		virtual KVOID	PostDestroy();
		
		virtual KVOID	OnTimer();
		virtual KVOID	SetTimer(KFLOAT fTimeStep);
		virtual KVOID	KillTimer();
		
		// 胜利条件处理
// 		virtual KVOID	DoTotalling();
// 		virtual KVOID	SetVictoryFactors(KINT nCount);
// 		virtual KVOID	AddVictoryFactor();

		// 累加成功条件
		KVOID AddSuccessFactor();

		// 累加失败条件
		KVOID AddFailureFactor();

		// 获得成功条件
		KVOID SetSuccessFactors(KINT nCount) { m_nSuccessFactors = nCount; }

		// 获得失败条件
		KVOID SetFailureFactors(KINT nCount) { m_nFailureFactors = nCount; }
	
		// 
		KVOID SetTaskExplain(KCSTR& sExplain) { m_sExplain = sExplain; }

	protected:
		KVOID			EV_OnTimer(EventPtr spEV);

	protected:
		KBOOL	m_bTimerEnable;
		KFLOAT	m_fTimeStep;
		
		KSTR	m_sExplain;
		KINT	m_nSuccessFactors;
		KINT	m_nFailureFactors;
	};
};

