#pragma once

#include "EffectManager.h"


namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	// 动态模糊特效
	class EffectMotionBlur : public EffectCompositor
	{
	public:
		EffectMotionBlur():EffectCompositor("MotionBlur"){}
		virtual KBOOL Initialize();

	};
}