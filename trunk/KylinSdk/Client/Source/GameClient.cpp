#include "cltpch.h"
#include "GameClient.h"
#include "rOgreRoot.h"
#include "ClLobby.h"
#include "DataManager.h"
#include "DataLoader.h"
#include "CommandLineParser.h"
#include "KylinRoot.h"
#include "ClGame.h"
#include "ClLobby.h"
#include "ClRegisterClass.h"


KVOID Kylin::GameClient::Entrance(KCHAR *argv)
{
	//////////////////////////////////////////////////////////////////////////
	ClRegisterClasses();
	//////////////////////////////////////////////////////////////////////////
	// 加载数据表
	DataTableLoading();
	//////////////////////////////////////////////////////////////////////////
	// 命令行解析
	if (strcmp(argv,"-m 0") == 0)
		SwitchStatus(KNEW ClGame());
	else
 		SwitchStatus(KNEW ClLobby());
}

KVOID Kylin::GameClient::Destroy()
{
	GameFrame::Destroy();
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

	if (DataManager::GetSingletonPtr()->GetGlobalValue("AVATAR_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));

	if (DataManager::GetSingletonPtr()->GetGlobalValue("ITEM_DB",sValue))
		DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::DataLoader(sValue));
}

extern int tolua_clscript_open(lua_State* tolua_S);
KVOID Kylin::GameClient::OpenScriptBinding( lua_State *L )
{
	GameFrame::OpenScriptBinding(L);

	tolua_clscript_open(L);
}