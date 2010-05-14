#pragma once

#include "SceneLoader.h"

namespace Kylin
{
	class ClSceneLoader : public SceneLoader
	{
	public:

		virtual KBOOL LoadScene(KSTR mSceneFile){ return true; }
		virtual KBOOL LoadPlayer();
		virtual KVOID LoadLevel();
	};
}