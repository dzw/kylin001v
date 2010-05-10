#pragma once

//#include "Engine.h"
#include "rOgreRoot.h"

namespace Kylin
{
	class SceneLoader
	{
	public:
		SceneLoader()
		{
			m_pTerrainGroup = NULL;
		}
		
		virtual ~SceneLoader()
		{
			if(m_pTerrainGroup)
			{
				OGRE_DELETE m_pTerrainGroup;
			}
		}

		virtual KBOOL Load(KSTR mSceneFile) = 0;
		virtual KVOID Unload(SceneHag* pHag){}

		Ogre::TerrainGroup* getTerrainGroup() { return m_pTerrainGroup; }

	protected:
		Ogre::TerrainGroup* m_pTerrainGroup;
	};
}

