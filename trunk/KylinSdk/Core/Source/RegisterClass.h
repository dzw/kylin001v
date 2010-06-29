#pragma once

#include "ClassFactory.h"

//tolua_begin
enum ClassID
{
	id_entity,
	id_level,
	id_character,
	id_spawner,
	id_npc,

	id_factor,

	id_last,
};
//tolua_end

typedef Kylin::CClassFactory< KINT, Kylin::Entity> EntitiesFactory;

#define RegEntity(classname) EntitiesFactory::RegisterCreatorFunction(classname::m_RTTI.GetClassID(), classname::CreateInstance); \
	classname::ConstructFuncMap();

extern KVOID RegisterClasses();
