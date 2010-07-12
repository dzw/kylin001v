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
		virtual KVOID	Destroy();
		
		virtual KVOID	OnTimer();
		virtual KVOID	SetTimer(KFLOAT fTimeStep);
		virtual KVOID	KillTimer();
		
		// ʤ����������
// 		virtual KVOID	DoTotalling();
// 		virtual KVOID	SetVictoryFactors(KINT nCount);
// 		virtual KVOID	AddVictoryFactor();

	protected:
		KVOID			EV_OnTimer(EventPtr spEV);
		
	protected:
		KBOOL	m_bTimerEnable;
		KFLOAT	m_fTimeStep;
//		KINT	m_nVictoryFactors;
	};
};

