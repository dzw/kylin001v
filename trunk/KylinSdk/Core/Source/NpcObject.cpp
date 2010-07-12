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
		// 设置移动速度
		m_kProperty.GetFloatValue("$Speed",m_pAIHandler->m_fSpeed);

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
		KUINT uKiller	= boost::get<unsigned int>(spEV->args[0]);
		KINT nMinDamage = boost::get<int>(spEV->args[1]);
		KINT nMaxDamage = boost::get<int>(spEV->args[2]);
		KINT nLevel		= 1;

		Kylin::Entity* pKiller = KylinRoot::GetSingletonPtr()->GetEntity(uKiller);
		if (pKiller)
			pKiller->GetPropertyRef().GetIntValue("$Level",nLevel);

		DamageUnit kUnit(nLevel,nMinDamage,nMaxDamage,1);
		
		DamageResult kResult = DamageSystem::Calculate(kUnit,this->GetID());
		if (kResult.mDIFF > 0)
		{
			// 执行对应的脚步函数
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_damage");
		}
		else
		{
			// 执行对应的脚步函数
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_killed");

			// 发送销毁消息
			EventPtr spEV(
				new Event(
				&ev_post_destroy, 
				Event::ev_timing, 
				3, 
				0, 
				NULL
				));

			KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
		}
	}
}