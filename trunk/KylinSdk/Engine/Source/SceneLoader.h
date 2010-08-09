#pragma once

//#include "Engine.h"
#include "rOgreRoot.h"
#include "SkyXWapper.h"


namespace Kylin
{
	class SceneLoader
	{
	public:
		SceneLoader() : m_pSkyWapper(NULL)
					  , m_pTerrainGroup(NULL)
		{}
		
		virtual KBOOL LoadScene(KSTR sSceneFile) = 0;
		virtual KBOOL LoadPlayer() = 0;
		virtual KVOID LoadLevel(KSTR sSceneName) = 0;
		virtual KVOID Tick(KFLOAT fElapsed)
		{
			SAFE_CALL(m_pSkyWapper,Tick(fElapsed));
		}

		virtual KVOID Unload(SceneHag* pHag)
		{
			SAFE_DEL(m_pSkyWapper);
			if (m_pTerrainGroup)
				OGRE_DELETE m_pTerrainGroup;

			// 清空场景管理器
  			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllEntities();
  			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllLights();
  			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllParticleSystems();
// 			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllMovableObjects();
//			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllManualObjects();
//			OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyAllAnimations();
			OgreRoot::GetSingletonPtr()->GetSceneManager()->clearScene();
		}

		Ogre::TerrainGroup* getTerrainGroup() { return m_pTerrainGroup; }

	protected:
		Ogre::TerrainGroup* m_pTerrainGroup;

		// test code 待天气系统完善
		//
		SkyXWapper*			m_pSkyWapper;
	};
}

