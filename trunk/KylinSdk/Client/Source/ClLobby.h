#pragma once

#include "GameStatus.h"
#include "GuiManager.h"

namespace Kylin
{
	class ClLobby : public GameStatus , public GuiShell
	{
	public:
		ClLobby();

		virtual KBOOL Initialize();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();
		
		virtual KVOID UiLoader();

	protected:
		Ogre::SceneNode* m_pNode;
		Ogre::Light*	 m_pLight;
	};
}