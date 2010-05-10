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
		
		enum ShowFlag
		{
			SF_NODE_,
			SF_PREPARE_,
			SF_SHOWING_,
		};

	protected:
		KVOID	OnPrepare(KFLOAT fElapsed);
		KVOID	OnShowing(KFLOAT fElapsed);
		
		virtual Showgirl* SpawnActor(const KUINT& uGid);

	protected:
		virtual KVOID OnLButtonDown(KINT nX, KINT nY);

	protected:
		Ogre::Camera*	 m_pCamera;
		Ogre::SceneNode* m_pNode;
		Ogre::Light*	 m_pLight;

		ShowFlag		m_eShowFlag;
		KUINT			m_uSelectedIndex;
		KVEC<Showgirl*>	m_kActorVec;
	};
}