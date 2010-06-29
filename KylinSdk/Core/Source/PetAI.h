#pragma once

#include "AI.h"

namespace Kylin
{
	class PetAI : public BaseAI
	{
	public:
		PetAI(NpcObject* pHost);
		~PetAI();

		virtual KBOOL Init();
		virtual KVOID Tick(KFLOAT fElapsed);

	protected:
		//-------------------------------------
		//½øÈëÄ³Âß¼­×´Ì¬
		RC_RESULT	Enter_UseSkill( KUINT uSkillId, KUINT uTarget, KPoint3 kPos, KFLOAT fDir );
		RC_RESULT	Enter_Follow( KUINT uTargetObj );

	protected:
		//-------------------------------------
		//Ä³Âß¼­×´Ì¬ÖÐµÄTickèå
		KBOOL		Tick_UseSkill( KFLOAT fElapsed );
		KBOOL		Tick_Idle( KFLOAT fElapsed );
	};

}