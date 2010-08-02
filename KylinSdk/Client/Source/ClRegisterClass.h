#pragma once

#include "RegisterClass.h"


//tolua_begin
enum ClClassID
{
	id_cl_entity = id_last + 1,

	id_bullet_factor,
	id_summon_factor,
	id_chain_factor,
	id_normal_factor,
	id_collapsar_factor,
	
	id_level_cl,
	id_player,
};
//tolua_end

extern KVOID ClRegisterClasses();
