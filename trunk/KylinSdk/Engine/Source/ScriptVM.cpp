#include "engpch.h"
#include ".\scriptvm.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "tolua++.h"
#include "log_t.h"

#define DEBUG_STACK 0

//end of tolua bind declare
extern KVOID tolua_open_binding(lua_State * L);
static KVOID stackDump(lua_State *L);

//error_msg handling function
static KVOID error_msg(KBOOL bUseAssert, KCCHAR * pacFmt, ...)
{
#if defined(_DEBUG)
	KCHAR acTemp[2048];

	va_list args;
	va_start(args, pacFmt);
	vsprintf_s(acTemp, pacFmt, args);
	va_end(args);
// 	if(bUseAssert)
// 		SP_ASSERT(false, acTemp);
	syslog->error(acTemp);
#endif
}

static KVOID report_last_error(lua_State *L, KBOOL bUseAssert)
{
	lua_getglobal(L, "_ALERT");
	error_msg(bUseAssert, "%s\n", lua_tostring(L, -2));
	error_msg(bUseAssert, "%s\n", lua_tostring(L, -1));
	lua_pop(L, 2);  /* remove error_msg message and _ALERT */
}

static KVOID stackDump (lua_State *L) {
	KINT i;
	KINT top = lua_gettop(L);
	for (i = 1; i <= top; i++) { /* repeat for each level */
		KINT t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN: /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER: /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		default: /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

namespace Kylin
{

ScriptVM::ScriptVM(KVOID)
{
}

ScriptVM::~ScriptVM(KVOID)
{
}

static KINT PrintStringList ( lua_State * L ){
	KINT n = lua_gettop(L);  /* number of arguments */
	KINT i;
	lua_getglobal(L, "tostring");
	string out;
	for (i=1; i<=n; i++) {
		KCCHAR *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, "`tostring' must return a string to `print'");
		if (i>1) 
		{
			out+="\t";
			out+=s;
		}
		else
			out+=s;

		lua_pop(L, 1);  /* pop result */
	}
	out+="\n";
	syslog->debug("%s",out.c_str());

	return 0;
}

KBOOL ScriptVM::Init(KVOID)
{
	L = lua_open();			

	luaL_openlibs(L);

//	luaopen_base(L);
//	luaopen_io(L);
//	luaopen_table(L);
//	luaopen_math(L);
//	luaopen_string(L);
//	luaopen_debug(L);

	lua_gc(L,LUA_GCCOLLECT, 200);//lua_setgcthreshold(L, 200); //200k garbage collection threshold
	lua_register(L,"print",PrintStringList);

	return true;
}

KVOID ScriptVM::Destroy(KVOID)
{
	lua_close(L);
}

KVOID ScriptVM::ExecuteScriptFile(KCCHAR * sScriptName, KBOOL bForceReload /* = false*/, KBOOL bAssertOnError /*= true*/)
{
	KINT nSize1 = lua_gettop(L);
	//get chunk name as modified script name
	KSTR sChunkName(sScriptName);
	for(KUINT i=0; i< sChunkName.length(); i++)
	{
		if(sChunkName[i] == '/' || sChunkName[i] == '.')
			sChunkName[i] = '_';
	}

	//get the chunk global
	lua_getglobal(L, sChunkName.c_str());
	if( bForceReload || !lua_isfunction(L, -1) )//if force reload or not found
	{
		luaL_loadfile(L, sScriptName);
		lua_setglobal(L, sChunkName.c_str());
		lua_getglobal(L, sChunkName.c_str());

	}
	if( lua_pcall(L, 0, 0, 0) != 0)
	{
		error_msg(bAssertOnError, "error executing script file %s: ", sScriptName);
		report_last_error(L, bAssertOnError);
	}

failed:
	lua_settop(L, nSize1);
}

KVOID ScriptVM::ExecuteScript(KCCHAR * sScript, KBOOL bAssertOnError)
{
	KINT status=luaL_loadbuffer(L,sScript,strlen(sScript),sScript);
	if(status)
	{
		report_last_error(L, bAssertOnError);
	}
	else
	{
		status = lua_pcall(L, 0, LUA_MULTRET, 0);  /* call main */
		if(status)
			report_last_error(L, bAssertOnError);
	}
}
/*
 *	Execute Script Function func in the script. copy/pasted from the book "programming in LUA"
 */
KVOID ScriptVM::ExecuteScriptFunc(const std::vector<KCCHAR *>&modules, KCCHAR * func, KBOOL bAllowNonexist, KCCHAR * sig, ...)
{
	KINT nSize1 = lua_gettop(L);

	va_list	vl;
	KINT	narg, nres;	/* number of arguments and results */
	va_start(vl, sig);

	//get the actual function
	if(modules.empty()) //func is global
	{
		lua_getglobal(L, func);
	}
	else
	{
		//trace down the modules
		std::vector<KCCHAR *>::const_iterator it = modules.begin();
		//get the global module name or the actual function name if there is no module
		lua_getglobal(L, *it);
		if(!lua_istable(L, -1))
		{
			if(!bAllowNonexist)
				error_msg(true, "ExecuteScriptFunc: Invalid table name: %s\n", *it );
			goto failed;
		}

		for( ++it; it != modules.end(); ++it)
		{
			lua_pushstring(L, *it);
			lua_gettable(L, -2);
			if(!lua_istable(L, -1))
			{
				if(!bAllowNonexist)
					error_msg(true, "ExecuteScriptFunc: Invalid table name: %s\n", *it );
				goto failed;
			}
		}
		//get the func
		lua_pushstring(L, func);
		lua_gettable(L, -2);
		if(!lua_isfunction(L, -1))
		{
			if(!bAllowNonexist)
				error_msg(true, "ExecuteScriptFunc: Invalid function name: %s\n", func);
			goto failed;
		}
	}

	/* push	arguments */
	narg = 0;
	while (*sig){ /* push arguments	*/
		switch (*sig++){
			case 'd': /* KDOUBLE	argument */
				lua_pushnumber(L, va_arg(vl, KDOUBLE));
				break;
			case 'i': /* KINT argument */
				lua_pushnumber(L, va_arg(vl, KINT));
				break;
			case 's': /* string	argument */
				lua_pushstring(L, va_arg(vl, KCHAR *));
				break;
			case 'b': /* boolean argument */
				lua_pushboolean(L, va_arg(vl, KBOOL));
				break;
			case 'u': /* light user data */
				lua_pushlightuserdata(L, va_arg(vl, KVOID *));
				break;
			case '>':
				goto endwhile;
			default:
				error_msg(true, "invalid option (%c)\n",	*(sig -	1));
				goto failed;
		}
		narg++;
		luaL_checkstack(L, 1, "too many	arguments");
	}endwhile:
	/* do the call */
	nres = strlen(sig);	/* number of expected results */
	if (lua_pcall(L, narg, nres, 0)	!= 0) /* do	the	call */
	{
		report_last_error(L, true);
		goto failed;
	}
	/* retrieve	results	*/
	nres = -nres; /* stack index of	first result */
	while (*sig)
	{ /* get results */
		switch (*sig++)
		{
			case 'd': /* KDOUBLE	result */
				if (!lua_isnumber(L, nres))
					error_msg(true, "wrong	result type\n");
				*va_arg(vl,	KDOUBLE *) =	lua_tonumber(L,	nres);
				break;
			case 'i': /* KINT result	*/
				if (!lua_isnumber(L, nres))
					error_msg(true, "wrong	result type\n");
				*va_arg(vl,	KINT	*) = (KINT)lua_tonumber(L, nres);
				break;
			case 's': /* string	result */
				if (!lua_isstring(L, nres))
					error_msg(true, "wrong	result type\n");
				*va_arg(vl,	KCCHAR **) = lua_tostring(L, nres);
				break;
			case 'b': /* boolean argument */
				if (!lua_isboolean(L, nres))
					error_msg(true, "wrong	result type\n");
				*va_arg(vl,	KBOOL *) = ( 0 != lua_toboolean(L, nres));
				break;
			case 'u': /* light user data */
				if (!lua_isuserdata(L, nres))
					error_msg(true, "wrong	result type\n");
				*va_arg(vl,	KVOID **) = lua_touserdata(L, nres);
				break;
			default:
				error_msg(true, "invalid option (%c)\n",	*(sig -	1));
		}
		nres++;
	}

failed:
	va_end(vl);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	
	if(nSize1 != nSize2)
		stackDump(L);
#endif
}

KVOID ScriptVM::ExposeGlobalUserdata( KVOID * va, KCCHAR * name, KCCHAR * type)
{
	KINT nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before ExposeGlobalUserdata = %d\n", nSize1);
#endif

	tolua_pushusertype(L, va, type);
	lua_setglobal(L, name);

	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	printf("debug lua: stack size after ExposeGlobalUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
}

KVOID * ScriptVM::GetGlobalUserdata(KCCHAR * name, KCCHAR * verify_type /*= NULL*/)
{
	KINT nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before GetGlobalUserdata = %d\n", nSize1);
#endif
	
	lua_getglobal(L, name);

	//verify type
	if(verify_type)
	{
		tolua_Error tolua_err;
		if (
			!tolua_isusertype(L,1,verify_type,0,&tolua_err) ||
			!tolua_isnoobj(L,2,&tolua_err)
			)
		{
			tolua_error(L,"#ferror in function 'ScriptVM::GetGlobalUserdata'.",&tolua_err);
			goto failed;
		}
	}

	KVOID * pRet = tolua_tousertype(L, -1, 0);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetGlobalUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	return pRet;

failed:
	//lua_settop(L,0);
	lua_settop(L, nSize1);
	return NULL;
}

KDOUBLE ScriptVM::GetGlobalNumber(KCCHAR * name)
{
	KINT nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before GetGlobalUserdata = %d\n", nSize1);
#endif

	lua_getglobal(L, name);

	KDOUBLE ret = tolua_tonumber(L, -1, 0);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetGlobalUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	return ret;
}

KVOID * ScriptVM::GetUserdata(const std::vector<KCCHAR *>& modules, KCCHAR * name, KCCHAR * verify_type/*= NULL*/)
{
	KINT nSize1 = lua_gettop(L);
#if DEBUG_STACK
	printf("debug lua: stack size before GetUserdata = %d\n", nSize1);
#endif
	if(modules.empty()) //userdata is global
	{
		lua_getglobal(L, name);
	}
	else
	{
		//trace down the modules
		std::vector<KCCHAR *>::const_iterator it = modules.begin();
		//get the global module name or the actual function name if there is no module
		lua_getglobal(L, *it);
		if(!lua_istable(L, -1))
		{
			error_msg(true, "GetUserdata: Invalid table name: %s\n", *it );
			goto failed;
		}

		for( ++it; it != modules.end(); ++it)
		{
			lua_pushstring(L, *it);
			lua_gettable(L, -2);
			if(!lua_istable(L, -1))
			{
				error_msg(true, "GetUserdata: Invalid table name: %s\n", *it );
				goto failed;
			}
		}
		//get the data
		lua_pushstring(L, name);
		lua_gettable(L, -2);
	}

	KVOID * pRet = tolua_tousertype(L, -1, 0);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	return pRet;

failed:
	lua_settop(L, nSize1);
	return NULL;
}

//由tolua注册过的类名创建对象
KVOID *	ScriptVM::CreateObjectByTypeName(KCCHAR * sTypeName)
{
	//处理lua脚本
	KINT nSize = (KINT)strlen(sTypeName)+20;
	KBYTE* buffer = KNEW KBYTE[nSize];
	sprintf((KCHAR*)buffer,"pMyCreatedObj=%s:KNEW()",sTypeName);
	
	//执行脚本
	ExecuteScript((KCHAR*)buffer);

	KVOID* ret=GetGlobalUserdata("pMyCreatedObj");

	return ret;
}

//获得全局表中常量
KDOUBLE ScriptVM::GetGlobalTableNumber(KCCHAR *sTableName,KCCHAR* key)
{
	KINT nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before GetGlobalUserdata = %d\n", nSize1);
#endif
	lua_getglobal(L,sTableName);
	if(!lua_istable(L,-1))
	{
		error_msg(true, "GetGlobalTableNumber: %s isn't a Lua Table.",sTableName);
		goto failed;
	}

	lua_pushstring(L,key);
	lua_gettable(L,-2);
	if(!lua_isnumber(L,-1))
	{
		error_msg(true, "GetGlobalTableNumber: %s isn't a number.",key);
		goto failed;
	}
	KDOUBLE ret = lua_tonumber(L,-1);
	lua_settop(L,nSize1);
#if DEBUG_STACK
	//debug
	KINT nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	
	return ret;

failed:
	lua_settop(L,nSize1);
	return 0;
}
}