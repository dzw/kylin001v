#pragma once

#include "rOgreRoot.h"
#include "CameraControl.h"
#include "EffectManager.h"
#include "Profiling.h"


namespace Kylin
{
	class CommandInterpreter
	{
	public:
		// execute the command
		virtual KVOID  Execute(KSTR sCmdLine) = 0;

		// [0] is command, another is arguments
		virtual Ogre::vector<Ogre::String>::type Interpret(KSTR sCmdLine)
		{
			Ogre::vector< Ogre::String >::type kOgHeap;
			
			kOgHeap = Ogre::StringUtil::split(sCmdLine," ");
		
			return kOgHeap;
		}

	protected:
		
	};

	//-------------------------------------------------------------------------
	class CommandHandler : public CommandInterpreter
	{
	public:
		virtual KVOID  PrintHelpInfo()
		{
			DebugConsole* pConsole = GET_GUI_PTR(DebugConsole);
			if (pConsole)
			{
				pConsole->PrintLine("================================================================");
				pConsole->PrintLine(">help		- print help info");
				pConsole->PrintLine(">editor	- open editor window");
				pConsole->PrintLine(">camera	- set camera mode (free and chase)");
				pConsole->PrintLine(">effect	- debug look effect, (effname true/false)");
				pConsole->PrintLine(">profile	- profiling");
				//pConsole->PrintLine("================================================================");
			}
		}

		virtual KVOID  Execute(KSTR sCmdLine)
		{
			Ogre::vector<Ogre::String>::type kCmd = Interpret(sCmdLine);
			if(kCmd.size() > 0)
			{
				Ogre::StringUtil::toLowerCase(kCmd[0]);

				if (kCmd[0] == "exit")
				{
					Kylin::OgreRoot::GetSingletonPtr()->ShutDown();
				}
				else if (kCmd[0] == "editor")
				{
					// �򿪱༭����
				}
				else if (kCmd[0] == "camera")
				{
					if (kCmd.size() < 2) return;

					if (kCmd[1] == "free")
						Kylin::OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("Free");
					else if(kCmd[1] == "chase")
						Kylin::OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("ChasePerson");
				}
				else if (kCmd[0] == "effect")
				{
					if (kCmd.size() < 3) return;

					if (kCmd[2] == "true")
						EffectManager::GetSingletonPtr()->Activate(kCmd[1],true);
					else
						EffectManager::GetSingletonPtr()->Activate(kCmd[1],false);
				}
				else if (kCmd[0] == "help")
				{
					PrintHelpInfo();
				}
				else if (kCmd[0] == "profile")
				{
					//CProfiling::GetSingletonPtr()->Profiling();
				}
			}
		}
	};

}