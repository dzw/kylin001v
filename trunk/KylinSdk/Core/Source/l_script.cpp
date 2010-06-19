/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on 06/17/10 13:11:24.
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
using namespace Script;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: set_fog_color */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_color00
static int tolua_script_set_fog_color00(lua_State* tolua_S)
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
  float r = ((float)  tolua_tonumber(tolua_S,1,0));
  float g = ((float)  tolua_tonumber(tolua_S,2,0));
  float b = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   set_fog_color(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_fog_distance */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_distance00
static int tolua_script_set_fog_distance00(lua_State* tolua_S)
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
  float d = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_fog_distance(d);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_distance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_fog_intensity */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_intensity00
static int tolua_script_set_fog_intensity00(lua_State* tolua_S)
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
  float i = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_fog_intensity(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_intensity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_fog_near */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_near00
static int tolua_script_set_fog_near00(lua_State* tolua_S)
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
  float n = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_fog_near(n);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_near'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_fog_far */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_far00
static int tolua_script_set_fog_far00(lua_State* tolua_S)
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
  float f = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_fog_far(f);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_far'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_fog_enable */
#ifndef TOLUA_DISABLE_tolua_script_set_fog_enable00
static int tolua_script_set_fog_enable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isboolean(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  bool b = ((bool)  tolua_toboolean(tolua_S,1,0));
  {
   set_fog_enable(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_fog_enable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_camera_near */
#ifndef TOLUA_DISABLE_tolua_script_set_camera_near00
static int tolua_script_set_camera_near00(lua_State* tolua_S)
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
  float n = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_camera_near(n);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_camera_near'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_camera_far */
#ifndef TOLUA_DISABLE_tolua_script_set_camera_far00
static int tolua_script_set_camera_far00(lua_State* tolua_S)
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
  float f = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_camera_far(f);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_camera_far'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_visible_distance */
#ifndef TOLUA_DISABLE_tolua_script_set_visible_distance00
static int tolua_script_set_visible_distance00(lua_State* tolua_S)
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
  float d = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   set_visible_distance(d);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_visible_distance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_viewport_back */
#ifndef TOLUA_DISABLE_tolua_script_set_viewport_back00
static int tolua_script_set_viewport_back00(lua_State* tolua_S)
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
  float r = ((float)  tolua_tonumber(tolua_S,1,0));
  float g = ((float)  tolua_tonumber(tolua_S,2,0));
  float b = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   set_viewport_back(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_viewport_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

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
  tolua_constant(tolua_S,"id_npc",id_npc);
  tolua_constant(tolua_S,"id_factor",id_factor);
  tolua_constant(tolua_S,"id_last",id_last);
  tolua_function(tolua_S,"set_fog_color",tolua_script_set_fog_color00);
  tolua_function(tolua_S,"set_fog_distance",tolua_script_set_fog_distance00);
  tolua_function(tolua_S,"set_fog_intensity",tolua_script_set_fog_intensity00);
  tolua_function(tolua_S,"set_fog_near",tolua_script_set_fog_near00);
  tolua_function(tolua_S,"set_fog_far",tolua_script_set_fog_far00);
  tolua_function(tolua_S,"set_fog_enable",tolua_script_set_fog_enable00);
  tolua_function(tolua_S,"set_camera_near",tolua_script_set_camera_near00);
  tolua_function(tolua_S,"set_camera_far",tolua_script_set_camera_far00);
  tolua_function(tolua_S,"set_visible_distance",tolua_script_set_visible_distance00);
  tolua_function(tolua_S,"set_viewport_back",tolua_script_set_viewport_back00);
  tolua_function(tolua_S,"lua_break",tolua_script_lua_break00);
  tolua_function(tolua_S,"set_next_anim",tolua_script_set_next_anim00);
  tolua_function(tolua_S,"set_effect",tolua_script_set_effect00);
  tolua_function(tolua_S,"set_global_effect",tolua_script_set_global_effect00);
  tolua_function(tolua_S,"exchange_avatar",tolua_script_exchange_avatar00);
  tolua_function(tolua_S,"add_pathway_pos",tolua_script_add_pathway_pos00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif

