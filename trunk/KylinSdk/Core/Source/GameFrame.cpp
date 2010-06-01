#include "corepch.h"
#include "gameFrame.h"
#include "KylinRoot.h"
#include "RegisterClass.h"
#include "GameStatus.h"
#include "Property.h"
#include "rPhyXSystem.h"


Kylin::GameFrame::GameFrame()
: m_pActiveStatus(NULL)
{
	
}

Kylin::GameFrame::~GameFrame()
{
	
}

KVOID Kylin::GameFrame::Run()
{
	//////////////////////////////////////////////////////////////////////////////
	AppFrame::Run();
	//////////////////////////////////////////////////////////////////////////////
}

KVOID Kylin::GameFrame::Destroy()
{
	if (m_pActiveStatus)
		m_pActiveStatus->Destroy();
	KDEL m_pActiveStatus;

	if (KylinRoot::Initialized())
		KDEL KylinRoot::GetSingletonPtr();

	if (PhyX::PhysicalSystem::Initialized())
		KDEL PhyX::PhysicalSystem::GetSingletonPtr();

	AppFrame::Destroy();
}

KVOID Kylin::GameFrame::OnIdle( KFLOAT fElapsed )
{
	AppFrame::OnIdle(fElapsed);
	
	SAFE_CALL(PhyX::PhysicalSystem::GetSingletonPtr(),Tick(fElapsed));
	
	SAFE_CALL(m_pActiveStatus,Tick(fElapsed));
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

	//////////////////////////////////////////////////////////////////////////
	if (!PhyX::PhysicalSystem::Initialized())
		KNEW PhyX::PhysicalSystem();
	PhyX::PhysicalSystem::GetSingletonPtr()->CreateMotionSimulator();
	PhyX::PhysicalSystem::GetSingletonPtr()->CreateCollisionMonitor();
	//////////////////////////////////////////////////////////////////////////
	if (!KylinRoot::Initialized())
		KNEW KylinRoot();

	//////////////////////////////////////////////////////////////////////////
	RegisterClasses();
}

KVOID Kylin::GameFrame::SwitchStatus( GameStatus* pStatus )
{
	PropertySet kSwitchInfo;
	if (m_pActiveStatus)
	{
		m_pActiveStatus->Deserialize(kSwitchInfo);
		m_pActiveStatus->Destroy();

		KDEL m_pActiveStatus;
	}
	
	//////////////////////////////////////////////////////////////////////////
	m_pActiveStatus = pStatus;
	if (m_pActiveStatus)
	{
		m_pActiveStatus->Serialize(kSwitchInfo);
		m_pActiveStatus->Initialize();
	}
}