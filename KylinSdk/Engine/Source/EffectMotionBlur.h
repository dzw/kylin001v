#pragma once

#include "EffectManager.h"


namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	// ��̬ģ����Ч
	class EffectMotionBlur : public EffectCompositor
	{
	public:
		EffectMotionBlur():EffectCompositor("MotionBlur"){}
		virtual KBOOL Initialize();

	};
}