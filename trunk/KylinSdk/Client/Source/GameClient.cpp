#include "cltpch.h"
#include "GameClient.h"
#include "rOgreRoot.h"
#include "ClLobby.h"
#include "DataManager.h"
#include "dbCharacterLoader.h"


KVOID Kylin::GameClient::Entrance()
{
	//////////////////////////////////////////////////////////////////////////
	// �������ݱ�
	DataManager::GetSingletonPtr()->InvokeLoader(KNEW Kylin::CharacterLoader());
	//////////////////////////////////////////////////////////////////////////

	SwitchStatus(KNEW ClLobby());
}