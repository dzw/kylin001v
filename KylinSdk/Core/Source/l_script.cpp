/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on 04/08/10 17:19:02.
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

/* function: lua_test */
#ifndef TOLUA_DISABLE_tolua_script_lua_test00
static int tolua_script_lua_test00(lua_State* tolua_S)
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
  int i = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   lua_test(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_test'.",&tolua_err);
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
  tolua_function(tolua_S,"lua_test",tolua_script_lua_test00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif
