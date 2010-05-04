#include "cltpch.h"
#include "ClLobby.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"


Kylin::ClLobby::ClLobby()
{
	m_eStatus = GS_LOBBY_;

	ttime = 5.0f;
}

KBOOL Kylin::ClLobby::Initialize()
{
	Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->InitShell(this);

	return true;
}

KVOID Kylin::ClLobby::Tick( KFLOAT fElapsed )
{
	if (ttime > 0)
	{
		ttime -= fElapsed;

		if (ttime < 0)
		{
			Kylin::KylinRoot::GetSingletonPtr()->SwitchStatus(KNEW GSGame());
		}
	}
}

KVOID Kylin::ClLobby::Destroy()
{

}

KVOID Kylin::ClLobby::UiLoader()
{
	
}

