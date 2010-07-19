/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on 07/19/10 09:46:35.
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
#include "ItemCell.h"
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

/* function: add_anim_queue */
#ifndef TOLUA_DISABLE_tolua_script_add_anim_queue00
static int tolua_script_add_anim_queue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  const char* sAnim = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   add_anim_queue(uEntID,sAnim);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_anim_queue'.",&tolua_err);
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
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
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
  bool bFlag = ((bool)  tolua_toboolean(tolua_S,6,0));
  {
   set_spawner(uEntID,fInterval,fDelay,nMaxCount,uSpawnID,bFlag);
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

/* function: set_timer */
#ifndef TOLUA_DISABLE_tolua_script_set_timer00
static int tolua_script_set_timer00(lua_State* tolua_S)
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
  float fTimeStep = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   set_timer(uEntID,fTimeStep);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_timer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: kill_timer */
#ifndef TOLUA_DISABLE_tolua_script_kill_timer00
static int tolua_script_kill_timer00(lua_State* tolua_S)
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
   kill_timer(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'kill_timer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: kill_character */
#ifndef TOLUA_DISABLE_tolua_script_kill_character00
static int tolua_script_kill_character00(lua_State* tolua_S)
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
  unsigned int uKiller = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   kill_character(uEntID,uKiller);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'kill_character'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: destroy_entity */
#ifndef TOLUA_DISABLE_tolua_script_destroy_entity00
static int tolua_script_destroy_entity00(lua_State* tolua_S)
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
  float fTimeDelay = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   destroy_entity(uEntID,fTimeDelay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy_entity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: throw_item */
#ifndef TOLUA_DISABLE_tolua_script_throw_item00
static int tolua_script_throw_item00(lua_State* tolua_S)
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
  unsigned int uItem = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  float fTimeDelay = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   throw_item(uEntID,uItem,fTimeDelay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'throw_item'.",&tolua_err);
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
  tolua_constant(tolua_S,"id_item",id_item);
  tolua_constant(tolua_S,"id_last",id_last);
  tolua_function(tolua_S,"lua_break",tolua_script_lua_break00);
  tolua_function(tolua_S,"set_next_anim",tolua_script_set_next_anim00);
  tolua_function(tolua_S,"add_anim_queue",tolua_script_add_anim_queue00);
  tolua_function(tolua_S,"add_effect",tolua_script_add_effect00);
  tolua_function(tolua_S,"add_global_effect",tolua_script_add_global_effect00);
  tolua_function(tolua_S,"exchange_avatar",tolua_script_exchange_avatar00);
  tolua_function(tolua_S,"query_near_foeman",tolua_script_query_near_foeman00);
  tolua_function(tolua_S,"set_spawner",tolua_script_set_spawner00);
  tolua_function(tolua_S,"set_translate",tolua_script_set_translate00);
  tolua_function(tolua_S,"set_timer",tolua_script_set_timer00);
  tolua_function(tolua_S,"kill_timer",tolua_script_kill_timer00);
  tolua_function(tolua_S,"kill_character",tolua_script_kill_character00);
  tolua_function(tolua_S,"destroy_entity",tolua_script_destroy_entity00);
  tolua_function(tolua_S,"throw_item",tolua_script_throw_item00);
  tolua_constant(tolua_S,"AT_IMM",AT_IMM);
  tolua_constant(tolua_S,"AT_POS",AT_POS);
  tolua_constant(tolua_S,"AT_TAR",AT_TAR);
  tolua_constant(tolua_S,"IT_AVATAR",IT_AVATAR);
  tolua_constant(tolua_S,"IT_ACTION",IT_ACTION);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif

