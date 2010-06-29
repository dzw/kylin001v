#pragma once

#include "CommandHandler.h"

namespace Kylin
{
	class UserCommandHandler : public CommandHandler
	{
	public:
		virtual KVOID  PrintHelpInfo()
		{
			CommandHandler::PrintHelpInfo();

			DebugConsole* pConsole = (DebugConsole*)(OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("DebugConsole"));
			if (pConsole)
			{
				pConsole->PrintLine(">change_scene - switch scene (scene id)");
				pConsole->PrintLine("================================================================");
			}
		}

		virtual KVOID  Execute(KSTR sCmdLine)
		{
			CommandHandler::Execute(sCmdLine);

			Ogre::vector<Ogre::String>::type kCmd = Interpret(sCmdLine);
			if(kCmd.size() > 0)
			{
				Ogre::StringUtil::toLowerCase(kCmd[0]);

				if (kCmd[0] == "change_scene")
				{
					if (kCmd.size() < 2) return;
					KUINT uSceneID = atoi(kCmd[1].data());
					if (uSceneID > 0)
						KylinRoot::GetSingletonPtr()->SwitchScene(uSceneID);
				}
				else if (kCmd[0] == "show_box")
				{
					if (kCmd.size() < 2) return;
					
					if (kCmd[1] == "true")
						KylinRoot::GetSingletonPtr()->DebugShowBoundingBox(true);
					else
						KylinRoot::GetSingletonPtr()->DebugShowBoundingBox(false);
				}
			}
		}
	};
}