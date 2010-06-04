//--------------------------------------------------------------------------------
//
// Filename   : Assert.h
//
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//
// 
// 
//
//--------------------------------------------------------------------------------
VOID __assert__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr) ;
VOID __assertex__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
VOID __assertspecial__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
VOID __messagebox__(const CHAR*msg ) ;
//--------------------------------------------------------------------------------
//
// 
// 
//
//--------------------------------------------------------------------------------
VOID __protocol_assert__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr) ;

// #ifdef NDEBUG
// #define Assert(expr) ((VOID)0)
// #define AssertEx(expr,msg) ((VOID)0)
// #define AssertSpecial(expr,msg) ((VOID)0)
// #define MyMessageBox(msg) ((VOID)0)
// #elif __LINUX__
// #define Assert(expr) {if(!(expr)){__assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr);}}
// #define ProtocolAssert(expr) ((VOID)((expr)?0:(__protocol_assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr),0)))
// #define AssertEx(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
// #define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
// #define AssertExPass(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
// #define MyMessageBox(msg) ((VOID)0)
// #elif __WIN_CONSOLE__ || __WIN32__ || __WINDOWS__
#ifdef _DEBUG
#define Assert(expr) ((VOID)((expr)?0:(__assert__(__FILE__,__LINE__,__FUNCTION__,#expr),0)))
#define AssertEx(expr,msg) ((VOID)((expr)?0:(__assertex__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#define AssertSpecial(expr,msg) ((VOID)((expr)?0:(__assertspecial__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#define MyMessageBox(msg) __messagebox__(msg)
#else
#define Assert(expr) ((VOID)0)
#define AssertEx(expr,msg) ((VOID)0)
#define AssertSpecial(expr,msg) ((VOID)0)
#define MyMessageBox(msg) ((VOID)0)
#endif // _DEBUG
// #elif __MFC__
// #define Assert(expr) ASSERT(expr)
// #define AssertEx(expr,msg) ((VOID)0)
// #define AssertSpecial(expr,msg) ((VOID)0)
// #define MyMessageBox(msg) ((VOID)0)
// #endif

