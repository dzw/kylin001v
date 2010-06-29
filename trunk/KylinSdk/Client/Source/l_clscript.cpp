/*
** Lua binding: clscript
** Generated automatically by tolua++-1.0.92 on 06/25/10 16:33:05.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_clscript_open (lua_State* tolua_S);

#include "cltpch.h"
#include "tolua++.h"
#pragma warning(disable : 4800) //forcing value to bool warning
#include "clScriptFunction.h"
#include "ClRegisterClass.h"
using namespace Script;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: ui_reg_char_info */
#ifndef TOLUA_DISABLE_tolua_clscript_ui_reg_char_info00
static int tolua_clscript_ui_reg_char_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   ui_reg_char_info(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ui_reg_char_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: to_learn_skill */
#ifndef TOLUA_DISABLE_tolua_clscript_to_learn_skill00
static int tolua_clscript_to_learn_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int uActID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  char* chPos = ((char*)  tolua_tostring(tolua_S,3,0));
  {
   to_learn_skill(uEntID,uActID,chPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_learn_skill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: to_bind_weapon */
#ifndef TOLUA_DISABLE_tolua_clscript_to_bind_weapon00
static int tolua_clscript_to_bind_weapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int uWeaponID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* sNode = ((const char*)  tolua_tostring(tolua_S,3,0));
  {
   to_bind_weapon(uEntID,uWeaponID,sNode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_bind_weapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_clscript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"ui_reg_char_info",tolua_clscript_ui_reg_char_info00);
  tolua_function(tolua_S,"to_learn_skill",tolua_clscript_to_learn_skill00);
  tolua_function(tolua_S,"to_bind_weapon",tolua_clscript_to_bind_weapon00);
  tolua_constant(tolua_S,"id_cl_entity",id_cl_entity);
  tolua_constant(tolua_S,"id_bullet_factor",id_bullet_factor);
  tolua_constant(tolua_S,"id_summon_factor",id_summon_factor);
  tolua_constant(tolua_S,"id_chain_factor",id_chain_factor);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_clscript (lua_State* tolua_S) {
 return tolua_clscript_open(tolua_S);
};
#endif

