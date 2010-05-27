#pragma once

#include "SceneLoader.h"

namespace Kylin
{
	class SceneHag;
	class PlayerController;
	class ClSceneLoader : public SceneLoader
	{
	public:
		ClSceneLoader();
		
		virtual KVOID Unload(SceneHag* pHag);
		virtual KBOOL LoadScene(KSTR mSceneFile){ return true; }
		virtual KBOOL LoadPlayer();
		virtual KVOID LoadLevel();
		
		virtual KVOID Tick(KFLOAT fElapsed);

	protected:
		PlayerController*	m_pController;
	};
}