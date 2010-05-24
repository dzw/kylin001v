#pragma once

#include "InputListener.h"

namespace Kylin
{
	class Showgirl;
	class LobbyScene : public InputListener
	{
	public:
		LobbyScene();
		virtual ~LobbyScene();
		
		virtual KBOOL Initialize();
		virtual KBOOL LoadActors();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID SpawnActors();

	protected:
		virtual KVOID	SpawnScene();

	protected:
		Ogre::Camera*	 m_pCamera;
		Ogre::SceneNode* m_pNode;
		
		SceneLoader*	m_pSceneLoader;

		KUINT			m_uSelectedIndex;
		KVEC<Showgirl*>	m_kActorVec;
	};
}