/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on 06/03/10 19:32:40.
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
#include "ScriptFunctions.h"
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

/* function: set_effect */
#ifndef TOLUA_DISABLE_tolua_script_set_effect00
static int tolua_script_set_effect00(lua_State* tolua_S)
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
   set_effect(uEntID,uEffectID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_effect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_global_effect */
#ifndef TOLUA_DISABLE_tolua_script_set_global_effect00
static int tolua_script_set_global_effect00(lua_State* tolua_S)
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
   set_global_effect(uEffectID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_global_effect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: bind_weapon */
#ifndef TOLUA_DISABLE_tolua_script_bind_weapon00
static int tolua_script_bind_weapon00(lua_State* tolua_S)
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
   bind_weapon(uEntID,uWeaponID,sNode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bind_weapon'.",&tolua_err);
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

/* Open function */
TOLUA_API int tolua_script_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"lua_break",tolua_script_lua_break00);
  tolua_function(tolua_S,"set_next_anim",tolua_script_set_next_anim00);
  tolua_function(tolua_S,"set_effect",tolua_script_set_effect00);
  tolua_function(tolua_S,"set_global_effect",tolua_script_set_global_effect00);
  tolua_function(tolua_S,"bind_weapon",tolua_script_bind_weapon00);
  tolua_function(tolua_S,"exchange_avatar",tolua_script_exchange_avatar00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif

