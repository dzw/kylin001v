#pragma once

#include "AI.h"
#include "Character.h"


namespace Kylin
{
	class NpcObject : public Character
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(NpcObject, Character)
		Declare_Entity_Creator(NpcObject)

		NpcObject();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID PostSpawn();

		virtual KVOID Destroy();
		
	public:
		BaseAI* GetAIHandler();
		KVOID	SetAIHandler(BaseAI* pAI);
		
		KUINT	GetMasterWorldID();
		KVOID	SetMaster(const Character* pChar);

	protected:
		BaseAI*		m_pAIHandler;
		KUINT		m_uMasterID;
	};
}

