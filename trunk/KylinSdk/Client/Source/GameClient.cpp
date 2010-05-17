#include "cltpch.h"
#include "GameClient.h"
#include "rOgreRoot.h"
#include "ClLobby.h"
#include "DataManager.h"
#include "KylinHelper.h"
#include "DataLoader.h"


KVOID Kylin::GameClient::Entrance()
{
	if (!KylinHelper::Initialized())
		KNEW KylinHelper();
	//////////////////////////////////////////////////////////////////////////
	// 加载数据表
	DataTableLoading();
	//////////////////////////////////////////////////////////////////////////

	SwitchStatus(KNEW ClLobby());
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
	
}