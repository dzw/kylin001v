#include "corepch.h"

#include ".\scriptfunctions.h"
#include "ScriptVM.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "AnimationProxy.h"


namespace Script
{

	void lua_break(const char * sDebugInfo)
	{
		OutputDebugString(sDebugInfo);
		DebugBreak();
	}
	

	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (pEnt)
		{
			pEnt->GetAnimationProxy()->Play(sAnim,fTimes < 0);
		}
	}

	extern void set_effect( unsigned int uEntID, unsigned int uEffectID)
	{
		int i = 0;
	}

	extern void set_global_effect( unsigned int uEffectID )
	{
		int i = 0;
	}
}