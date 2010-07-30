#include "corepch.h"
#include "GameStatus.h"
#include "WorldManager.h"
#include "rPhyXSystem.h"


#define KY_WORLD_CGF "world.xml"


Kylin::GSGame::GSGame()
: m_fGameTime(.0f)
{
	m_eStatus = GS_GAME_;
}


KBOOL Kylin::GSGame::Initialize()
{
	if (PhyX::PhysicalSystem::Initialized())
		PhyX::PhysicalSystem::GetSingletonPtr()->SetEnable(true);

	m_pWorldManager = KNEW Kylin::WorldManager();

	return m_pWorldManager->Initialize(KY_WORLD_CGF);
}

KVOID Kylin::GSGame::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pWorldManager,Tick(fElapsed));

	m_fGameTime += fElapsed;	
}

KVOID Kylin::GSGame::Destroy()
{
	SAFE_CALL(m_pWorldManager,Destroy());
	SAFE_DEL(m_pWorldManager);
}
