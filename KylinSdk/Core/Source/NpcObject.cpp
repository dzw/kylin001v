#include "corepch.h"
#include "NpcObject.h"
#include "RegisterClass.h"
#include "AI.h"


namespace Kylin
{
	BtImplementRTTI(NpcObject, Character, id_npc);

	Implement_Event_Handler(NpcObject, Character)
	{
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
		// ����Ĭ��AI
		m_pAIHandler = KNEW BaseAI(this);
		
		//-----------------------------------------------------------------
		// �����ƶ��ٶ�
		m_kProperty.GetFloatValue("$Speed",m_pAIHandler->m_fSpeed);

		return true;
	}

	KVOID NpcObject::Tick( KFLOAT fElapsed )
	{
		Character::Tick(fElapsed);
		
		if (!IsDead())
		{
			SAFE_CALL(m_pAIHandler,Tick(fElapsed));
		}
	}

	KVOID NpcObject::PostSpawn()
	{
		Character::PostSpawn();
		
		SAFE_CALL(m_pAIHandler,Init());
	}

	KVOID NpcObject::PostDestroy()
	{
		SAFE_DEL(m_pAIHandler);
		//-----------------------------------------------------------
		Character::PostDestroy();
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
}