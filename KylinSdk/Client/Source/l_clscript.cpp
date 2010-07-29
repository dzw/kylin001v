/*
** Lua binding: clscript
** Generated automatically by tolua++-1.0.92 on 07/27/10 14:34:15.
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
#include "GameDefine.h"
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
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int uEntID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int uActID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  bool toUi = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   to_learn_skill(uEntID,uActID,toUi);
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

/* function: set_default_action */
#ifndef TOLUA_DISABLE_tolua_clscript_set_default_action00
static int tolua_clscript_set_default_action00(lua_State* tolua_S)
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
  unsigned int uActID = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   set_default_action(uActID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_default_action'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_default_action_factory */
#ifndef TOLUA_DISABLE_tolua_clscript_set_default_action_factory00
static int tolua_clscript_set_default_action_factory00(lua_State* tolua_S)
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
   set_default_action_factory(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_default_action_factory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_ui_monster_hp */
#ifndef TOLUA_DISABLE_tolua_clscript_set_ui_monster_hp00
static int tolua_clscript_set_ui_monster_hp00(lua_State* tolua_S)
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
   set_ui_monster_hp(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_ui_monster_hp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_ui_player_hp */
#ifndef TOLUA_DISABLE_tolua_clscript_set_ui_player_hp00
static int tolua_clscript_set_ui_player_hp00(lua_State* tolua_S)
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
   set_ui_player_hp(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_ui_player_hp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: post_gameresult */
#ifndef TOLUA_DISABLE_tolua_clscript_post_gameresult00
static int tolua_clscript_post_gameresult00(lua_State* tolua_S)
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
  bool bFlag = ((bool)  tolua_toboolean(tolua_S,1,0));
  {
   post_gameresult(bFlag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'post_gameresult'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_pathway */
#ifndef TOLUA_DISABLE_tolua_clscript_set_pathway00
static int tolua_clscript_set_pathway00(lua_State* tolua_S)
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
  unsigned int uPathwayID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   set_pathway(uEntID,uPathwayID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_pathway'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_item */
#ifndef TOLUA_DISABLE_tolua_clscript_add_item00
static int tolua_clscript_add_item00(lua_State* tolua_S)
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
  unsigned int uItemGID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   add_item(uEntID,uItemGID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_item'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: addin_minimap */
#ifndef TOLUA_DISABLE_tolua_clscript_addin_minimap00
static int tolua_clscript_addin_minimap00(lua_State* tolua_S)
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
   addin_minimap(uEntID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addin_minimap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_success_factor */
#ifndef TOLUA_DISABLE_tolua_clscript_add_success_factor00
static int tolua_clscript_add_success_factor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   add_success_factor();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_success_factor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: add_failure_factor */
#ifndef TOLUA_DISABLE_tolua_clscript_add_failure_factor00
static int tolua_clscript_add_failure_factor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   add_failure_factor();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_failure_factor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_success_factors */
#ifndef TOLUA_DISABLE_tolua_clscript_set_success_factors00
static int tolua_clscript_set_success_factors00(lua_State* tolua_S)
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
  int nCount = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   set_success_factors(nCount);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_success_factors'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_failure_factors */
#ifndef TOLUA_DISABLE_tolua_clscript_set_failure_factors00
static int tolua_clscript_set_failure_factors00(lua_State* tolua_S)
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
  int nCount = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   set_failure_factors(nCount);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_failure_factors'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_task_explain */
#ifndef TOLUA_DISABLE_tolua_clscript_set_task_explain00
static int tolua_clscript_set_task_explain00(lua_State* tolua_S)
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
  const char* pExplain = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   set_task_explain(pExplain);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_task_explain'.",&tolua_err);
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
  tolua_function(tolua_S,"set_default_action",tolua_clscript_set_default_action00);
  tolua_function(tolua_S,"set_default_action_factory",tolua_clscript_set_default_action_factory00);
  tolua_function(tolua_S,"set_ui_monster_hp",tolua_clscript_set_ui_monster_hp00);
  tolua_function(tolua_S,"set_ui_player_hp",tolua_clscript_set_ui_player_hp00);
  tolua_function(tolua_S,"post_gameresult",tolua_clscript_post_gameresult00);
  tolua_function(tolua_S,"set_pathway",tolua_clscript_set_pathway00);
  tolua_function(tolua_S,"add_item",tolua_clscript_add_item00);
  tolua_function(tolua_S,"addin_minimap",tolua_clscript_addin_minimap00);
  tolua_function(tolua_S,"add_success_factor",tolua_clscript_add_success_factor00);
  tolua_function(tolua_S,"add_failure_factor",tolua_clscript_add_failure_factor00);
  tolua_function(tolua_S,"set_success_factors",tolua_clscript_set_success_factors00);
  tolua_function(tolua_S,"set_failure_factors",tolua_clscript_set_failure_factors00);
  tolua_function(tolua_S,"set_task_explain",tolua_clscript_set_task_explain00);
  tolua_constant(tolua_S,"id_cl_entity",id_cl_entity);
  tolua_constant(tolua_S,"id_bullet_factor",id_bullet_factor);
  tolua_constant(tolua_S,"id_summon_factor",id_summon_factor);
  tolua_constant(tolua_S,"id_chain_factor",id_chain_factor);
  tolua_constant(tolua_S,"id_normal_factor",id_normal_factor);
  tolua_constant(tolua_S,"id_collapsar_factor",id_collapsar_factor);
  tolua_constant(tolua_S,"id_player",id_player);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_clscript (lua_State* tolua_S) {
 return tolua_clscript_open(tolua_S);
};
#endif

