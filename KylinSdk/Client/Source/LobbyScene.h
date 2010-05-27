#pragma once

#include "Showgirl.h"

namespace Kylin
{
	class LobbyScene //: public InputListener
	{
	public:
		LobbyScene();
		virtual ~LobbyScene();
		
		virtual KBOOL Initialize();
		virtual KBOOL LoadActors();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID SpawnActor(KUINT uIndex);

	protected:
		virtual KVOID	SpawnScene();

	protected:
		Ogre::Camera*	 m_pCamera;
		Ogre::SceneNode* m_pNode;
		KPoint3			m_kPosition;
		KQuaternion		m_kQuaternion;
		KFLOAT			m_fScale;

		SceneLoader*	m_pSceneLoader;

		KUINT			m_uSelectedIndex;
		Showgirl*		m_pShowgirl;
		AnimQueue		m_kAnimQueue;
	};
}