#include "cltpch.h"
#include "GameClient.h"
#include "rOgreRoot.h"
#include "ClLobby.h"
#include "DataManager.h"
#include "KylinHelper.h"
#include "DataLoader.h"
#include "CommandLineParser.h"
#include "KylinRoot.h"
#include "ClGame.h"
#include "ClLobby.h"

// ���������в�����������
class GameCmdLineParam : public ICmdLineParam
{
public:
	virtual KBOOL Parse(KSTR argument,KSTR value)
	{
		if (argument == "m")
		{
			if (value == "0")
			{
				Kylin::KylinRoot::GetSingletonPtr()->GetGameFramePtr()->SwitchStatus(KNEW Kylin::ClGame());
				return false;
			}
		}
		
		Kylin::KylinRoot::GetSingletonPtr()->GetGameFramePtr()->SwitchStatus(KNEW Kylin::ClLobby());
		return false;
	}

	virtual KSTR GetError() { return ""; }
};

KVOID Kylin::GameClient::Entrance(KCHAR *argv)
{
	if (!KylinHelper::Initialized())
		KNEW KylinHelper();
	//////////////////////////////////////////////////////////////////////////
	// �������ݱ�
	DataTableLoading();
	//////////////////////////////////////////////////////////////////////////
	// �����н���
	CCommandLineParser kParser;
	GameCmdLineParam* pCmd = KNEW GameCmdLineParam(); 
	kParser.PutValueCommand("m",pCmd);
	kParser.SetDefaultCommand(pCmd);
	kParser.SetErrorCommand(pCmd);
	kParser.ParseArguments(argv);
	
	KDEL pCmd;
	pCmd = NULL;

// 	if (!bRet)
// 		SwitchStatus(KNEW ClLobby());
}

KVOID Kylin::GameClient::Destroy()
{
	GameFrame::Destroy();

	KDEL KylinHelper::GetSingletonPtr();
}

KVOID Kylin::GameClient::DataTableLoading()
{
	KSTR sValue;
	if (DataManager::GetSingletonPtr()->GetGlobalValue("CHAR_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));

	if (DataManager::GetSingletonPtr()->GetGlobalValue("ACTION_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));

	if (DataManager::GetSingletonPtr()->GetGlobalValue("EFFECT_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));
	
	if (DataManager::GetSingletonPtr()->GetGlobalValue("FACTOR_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));
}