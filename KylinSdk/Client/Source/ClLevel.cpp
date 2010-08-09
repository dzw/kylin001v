#include "cltpch.h"
#include "ClLevel.h"
#include "ClRegisterClass.h"

#include "RemoteEvents.h"
#include "KylinRoot.h"


namespace Kylin
{
	BtImplementRTTI(ClLevel, Level, id_level_cl);

	Implement_Event_Handler(ClLevel, Level)
	{
		{&ev_do_quit,				&EV_DoQuit},
		{NULL, NULL}
	};


	KVOID ClLevel::EV_DoQuit( EventPtr spEV )
	{
		KylinRoot::GetSingletonPtr()->QuitGame();
		//OgreRoot::GetSingletonPtr()->Pause();
	}
}