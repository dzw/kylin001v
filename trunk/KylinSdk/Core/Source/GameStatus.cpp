#include "corepch.h"
#include "GameStatus.h"
#include "WorldManager.h"


#define KY_WORLD_CGF "world.xml"
KBOOL Kylin::GSGame::Initialize()
{
	m_pWorldManager = KNEW Kylin::WorldManager();

	return m_pWorldManager->Initialize(KY_WORLD_CGF);
}

KVOID Kylin::GSGame::Tick( KFLOAT fElapsed )
{
	if (m_pWorldManager)
		m_pWorldManager->Tick(fElapsed);
}

KVOID Kylin::GSGame::Destroy()
{
	m_pWorldManager->Destroy();
	SAFE_DEL(m_pWorldManager);
}