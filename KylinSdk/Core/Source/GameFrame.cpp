#include "corepch.h"
#include "gameFrame.h"
#include "WorldManager.h"
#include "KylinRoot.h"
#include "RegisterClass.h"


#define KY_WORLD_CGF "world.xml"

Kylin::GameFrame::GameFrame()
{
	
}

Kylin::GameFrame::~GameFrame()
{
	
}

KVOID Kylin::GameFrame::Run()
{
	m_pWorldManager->Initialize(KY_WORLD_CGF);
	
	//////////////////////////////////////////////////////////////////////////////
	AppFrame::Run();
	//////////////////////////////////////////////////////////////////////////////
}

KVOID Kylin::GameFrame::Destroy()
{
	m_pWorldManager->Destroy();
	SAFE_DEL(m_pWorldManager);

	if (KylinRoot::Initialized())
		KDEL KylinRoot::GetSingletonPtr();

	AppFrame::Destroy();
}

KVOID Kylin::GameFrame::OnIdle( KFLOAT fElapsed )
{
	AppFrame::OnIdle(fElapsed);

	m_pWorldManager->Tick(fElapsed);
}

extern int tolua_script_open(lua_State* tolua_S);
KVOID Kylin::GameFrame::OpenScriptBinding(lua_State *L)
{
	AppFrame::OpenScriptBinding(L);

	tolua_script_open(L);
}

KVOID Kylin::GameFrame::CreateWidgets()
{
	AppFrame::CreateWidgets();

	if (!KylinRoot::Initialized())
		KNEW KylinRoot();

	m_pWorldManager = new WorldManager();

	//////////////////////////////////////////////////////////////////////////
	RegisterClasses();
}