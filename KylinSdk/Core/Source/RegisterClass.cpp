#include "CorePCH.h"
#include "Registerclass.h"

#include "Level.h"
#include "Character.h"
#include "Factor.h"
#include "NpcObject.h"


using namespace Kylin;

KVOID RegisterClasses()
{
	RegEntity(Kylin::Entity);
	RegEntity(Level);
	RegEntity(Character);
	RegEntity(Factor);
	RegEntity(NpcObject);

//	RegEntity(EntitySpawner);
// 	RegMessage(M_SpawnEntity);
// 	RegMessage(M_DestroyEntity);
// 	//
// 	RegMessage(M_RemoteEvent);
// 	RegMessage(M_ServerEvent);
// 	RegMessage(M_ClientEvent);
// 	RegMessage(M_SyncTime);
// 	RegMessage(M_ExecLevelScript);
// 	RegMessage(M_RestartGame);
}
