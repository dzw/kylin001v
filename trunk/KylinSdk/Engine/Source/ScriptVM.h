#pragma once


struct lua_State;

namespace Kylin
{
	class ScriptVM
	{
	public:
		ScriptVM(KVOID);
		virtual ~ScriptVM(KVOID);
		KBOOL	Init(KVOID);
		KVOID	Destroy(KVOID);
		KVOID	ExecuteScriptFile(KCCHAR * sScriptFileName, KBOOL bForceReload = false, KBOOL bAssertOnError = true);
		KVOID	ExecuteScript(KCCHAR * sScript, KBOOL bAssertOnError = true);
		KVOID	ExecuteScriptFunc(const std::vector<KCCHAR *>& modules, KCCHAR * func, KBOOL bAllowNonexist, KCCHAR * sig, ...);
		KVOID	ExposeGlobalUserdata(KVOID * va, KCCHAR * name, KCCHAR * type);
		KVOID *	GetGlobalUserdata(KCCHAR * name, KCCHAR * verify_type = NULL);
		KVOID *	GetUserdata(const std::vector<KCCHAR *>& modules, KCCHAR * name, KCCHAR * verify_type = NULL);
		KDOUBLE	GetGlobalNumber(KCCHAR * name);

		KVOID *	CreateObjectByTypeName(KCCHAR * sTypeName);//由tolua注册过的类名创建对象
		KDOUBLE	GetGlobalTableNumber(KCCHAR *sTableName,KCCHAR* key);
		lua_State * GetLuaState() {return L;}

	protected:
		lua_State* L;
	};
}


