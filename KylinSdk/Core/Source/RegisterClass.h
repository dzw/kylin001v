#pragma once

#include "ClassFactory.h"


//tolua_begin
enum ClassID
{
	id_entity,
	id_level,
	id_character,

	id_last,
};
//tolua_end

typedef Kylin::CClassFactory< KINT, Kylin::Entity> EntitiesFactory;

extern KVOID RegisterClasses();
