#pragma once

#include "SceneLoader.h"

namespace Kylin
{
	class PathwayLoader;
	class SceneHag;
	class PlayerController;
	class ClSceneLoader : public SceneLoader
	{
	public:
		ClSceneLoader();
		
		virtual KVOID Unload(SceneHag* pHag);
		virtual KBOOL LoadScene(KSTR mSceneFile){ return true; }
		virtual KBOOL LoadPlayer();
		virtual KVOID LoadLevel(KSTR sSceneName);
		
		virtual KVOID Tick(KFLOAT fElapsed);
		
		// test code
		PlayerController*	GetController();
		PathwayLoader*		GetPathwayLoader();

	protected:
		PlayerController*	m_pController;
		PathwayLoader*		m_pPathwayLoader;
	};
}