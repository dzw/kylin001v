/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on 06/29/10 15:50:05.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_script_open (lua_State* tolua_S);

#include "corepch.h"
#include "tolua++.h"
#pragma warning(disable : 4800) //forcing value to bool warning
#include "core.h"
#include "RegisterClass.h"
#include "ScriptFunctions.h"
#include "Action.h"
using namespace Script;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: lua_break */
#ifndef TOLUA_DISABLE_tolua_script_lua_break00
static int tolua_script_lua_break00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* sDebugInfo = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   lua_break(sDebugInfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_break'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_next_anim */
#ifndef TOLUA_DISABLE_tolua_script_set_next_anim00
static int tolua_script_set_next_anim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  const char* sAnim = ((const char*)  tolua_tostring(tolua_S,2,0));
  float fTimes = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   set_next_anim(uEntID,sAnim,fTimes);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_next_anim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_effect */
#ifndef TOLUA_DISABLE_tolua_script_add_effect00
static int tolua_script_add_effect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int uEffectID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   add_effect(uEntID,uEffectID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_effect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_global_effect */
#ifndef TOLUA_DISABLE_tolua_script_add_global_effect00
static int tolua_script_add_global_effect00(lua_State* tolua_S)
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
  unsigned int uEffectID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   add_global_effect(uEffectID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_global_effect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: exchange_avatar */
#ifndef TOLUA_DISABLE_tolua_script_exchange_avatar00
static int tolua_script_exchange_avatar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int uAvatarID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   exchange_avatar(uEntID,uAvatarID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exchange_avatar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_pathway_pos */
#ifndef TOLUA_DISABLE_tolua_script_add_pathway_pos00
static int tolua_script_add_pathway_pos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   add_pathway_pos(uEntID,x,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_pathway_pos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: query_near_foeman */
#ifndef TOLUA_DISABLE_tolua_script_query_near_foeman00
static int tolua_script_query_near_foeman00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   query_near_foeman(uEntID,r);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'query_near_foeman'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_spawner */
#ifndef TOLUA_DISABLE_tolua_script_set_spawner00
static int tolua_script_set_spawner00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  float fInterval = ((float)  tolua_tonumber(tolua_S,2,0));
  float fDelay = ((float)  tolua_tonumber(tolua_S,3,0));
  int nMaxCount = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned int uSpawnID = ((unsigned int)  tolua_tonumber(tolua_S,5,0));
  {
   set_spawner(uEntID,fInterval,fDelay,nMaxCount,uSpawnID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_spawner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_translate */
#ifndef TOLUA_DISABLE_tolua_script_set_translate00
static int tolua_script_set_translate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   set_translate(uEntID,x,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_translate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_script_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"id_entity",id_entity);
  tolua_constant(tolua_S,"id_level",id_level);
  tolua_constant(tolua_S,"id_character",id_character);
  tolua_constant(tolua_S,"id_spawner",id_spawner);
  tolua_constant(tolua_S,"id_npc",id_npc);
  tolua_constant(tolua_S,"id_factor",id_factor);
  tolua_constant(tolua_S,"id_last",id_last);
  tolua_function(tolua_S,"lua_break",tolua_script_lua_break00);
  tolua_function(tolua_S,"set_next_anim",tolua_script_set_next_anim00);
  tolua_function(tolua_S,"add_effect",tolua_script_add_effect00);
  tolua_function(tolua_S,"add_global_effect",tolua_script_add_global_effect00);
  tolua_function(tolua_S,"exchange_avatar",tolua_script_exchange_avatar00);
  tolua_function(tolua_S,"add_pathway_pos",tolua_script_add_pathway_pos00);
  tolua_function(tolua_S,"query_near_foeman",tolua_script_query_near_foeman00);
  tolua_function(tolua_S,"set_spawner",tolua_script_set_spawner00);
  tolua_function(tolua_S,"set_translate",tolua_script_set_translate00);
  tolua_constant(tolua_S,"AT_NONE",AT_NONE);
  tolua_constant(tolua_S,"AT_POINT",AT_POINT);
  tolua_constant(tolua_S,"AT_TARGET",AT_TARGET);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif

