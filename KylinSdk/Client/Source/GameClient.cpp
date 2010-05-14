#include "cltpch.h"
#include "GameClient.h"
#include "rOgreRoot.h"
#include "ClLobby.h"
#include "DataManager.h"
#include "dbCharacterLoader.h"
#include "KylinHelper.h"


KVOID Kylin::GameClient::Entrance()
{
	if (!KylinHelper::Initialized())
		KNEW KylinHelper();
	//////////////////////////////////////////////////////////////////////////
	// 加载数据表
	DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::CharacterLoader());
	//////////////////////////////////////////////////////////////////////////

	SwitchStatus(KNEW ClLobby());
}

KVOID Kylin::GameClient::Destroy()
{
	GameFrame::Destroy();

	KDEL KylinHelper::GetSingletonPtr();
}