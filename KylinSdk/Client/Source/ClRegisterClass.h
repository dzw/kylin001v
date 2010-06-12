#pragma once

#include "RegisterClass.h"


//tolua_begin
enum ClClassID
{
	id_cl_entity = id_last + 1,
	id_bullet_factor,

};
//tolua_end

extern KVOID ClRegisterClasses();
