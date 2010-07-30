#pragma once

#include "rOgreRoot.h"

namespace Kylin
{
	class Renderable
	{
	public:
		Renderable();
		~Renderable();

		virtual KVOID OnRenderStarted(KFLOAT fElapsed){}
		virtual KVOID OnRenderEnded(KFLOAT fElapsed){}
	};
}