#pragma once

#include "InputListener.h"

namespace Kylin
{
	class Showgirl;
	class Stage : public InputListener
	{
	public:
		Stage();
		virtual ~Stage();
		
		virtual KBOOL Initialize();
		virtual KBOOL LoadActors();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID SpawnActors();
	protected:
		KINT GetShowgirl(KSTR sName);

	protected:
		virtual KVOID OnLButtonDown(KINT nX, KINT nY);

	protected:
		Ogre::Camera*	 m_pCamera;
		Ogre::SceneNode* m_pNode;
		Ogre::Light*	 m_pLight;

		KUINT			m_uSelectedIndex;
		KVEC<Showgirl*>	m_kActorVec;
	};
}