#pragma once

#include "Entity.h"

namespace Kylin
{
	class LevelEventReg
	{
	public:
		KVOID RegisterEvent(KUINT id, const EventTemplate*);
		KVOID RegisterEvent(KUINT id, KCCHAR *);
		KVOID UnRegisterEvent(KUINT id, const EventTemplate*);
		KVOID UnRegisterEvent(KUINT id, KCCHAR *);
		KBOOL HasRegisteredEvent(KUINT id, const EventTemplate*);

	private:
		typedef std::vector<const EventTemplate*> TemplateVector;
		typedef stdext::hash_map<KUINT, TemplateVector> EventMapping;

		EventMapping m_mapping;
	};

	class LevelController
	{
	public:
		
 		KVOID Tick(KFLOAT fElapsed);
 
 		KVOID HandleEntityInit(Entity * pEntity);
 
 		KVOID EV_LVLCommonEvent(EventPtr ev);
 		KVOID EV_LVLInit(EventPtr ev);

		KVOID ExecuteScriptFunc(Entity * pEnt, const EventTemplate * pTemplate, KCCHAR * sig, ...);
		KVOID ExecuteScriptFunc(Entity * pEnt, KCCHAR * sScriptName, KCCHAR * sig, ...);
		KVOID ExecuteScriptFunc1Param(Entity * pEnt, KCCHAR * sScriptName, KCCHAR * sig ,KINT nParam);

	protected:
		LevelEventReg m_levelReg;
	};
}
