#include "corepch.h"
#include "NpcObject.h"
#include "RegisterClass.h"
#include "AI.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "DamageSystem.h"


namespace Kylin
{
	BtImplementRTTI(NpcObject, Character, id_npc);

	Implement_Event_Handler(NpcObject, Character)
	{
		{&ev_post_damage,			&EV_Damage},
		{&ev_post_killed,			&EV_Killed},
		{NULL, NULL}
	};

	NpcObject::NpcObject()
		: m_pAIHandler(NULL)
	{

	}

	KBOOL NpcObject::Init( const PropertySet& kProp )
	{
		if ( !Character::Init(kProp) )
			return false;
		
		//-----------------------------------------------------------------
		// 设置默认AI
		m_pAIHandler = KNEW BaseAI(this);
		
		//-----------------------------------------------------------------

		return true;
	}

	KVOID NpcObject::Tick( KFLOAT fElapsed )
	{
		Character::Tick(fElapsed);
		
		SAFE_CALL(m_pAIHandler,Tick(fElapsed));
	}

	KVOID NpcObject::PostSpawn()
	{
		Character::PostSpawn();
		
		SAFE_CALL(m_pAIHandler,Init());
	}

	KVOID NpcObject::Destroy()
	{
		SAFE_DEL(m_pAIHandler);
		//-----------------------------------------------------------
		Character::Destroy();
	}

	KVOID NpcObject::SetAIHandler( BaseAI* pAI )
	{
		Assert(pAI);
		SAFE_DEL(m_pAIHandler);
		m_pAIHandler = pAI;
		
		SAFE_CALL(m_pAIHandler,Init());
	}

	BaseAI* NpcObject::GetAIHandler()
	{
		return m_pAIHandler;
	}

	KVOID NpcObject::SetMaster( const Character* pChar )
	{
		m_uMasterID = pChar->GetID();
	}

	KUINT NpcObject::GetMasterWorldID()
	{
		return m_uMasterID;
	}

	KVOID NpcObject::EV_Damage( EventPtr spEV )
	{
		DamageUnit kUnit(1,1,1,1);
		
		if ( DamageSystem::Calculate(kUnit,this->GetID()) > 0 )
		{
			// 执行对应的脚步函数
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_damage");
		}
	}

	KVOID NpcObject::EV_Killed( EventPtr spEV )
	{

		// 执行对应的脚步函数
		KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_killed");
	}
}