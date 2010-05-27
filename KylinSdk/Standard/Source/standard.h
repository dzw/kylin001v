#pragma once

#include <assert.h>
#include <stdio.h>
#include <Stdlib.h>
#include <time.h>

#ifdef WIN32
	#include <windows.h>
	#include <iomanip>
	#include <psapi.h>
	#pragma comment(lib,"psapi.lib")
#endif

#include <stdarg.h>

#include <sstream>
#include <istream>
#include <fstream>
#include <string>
#include <map>
#include <hash_map>
#include <vector>
#include <list>
#include <set>

using namespace std;

#define NULL 0

// #define un_new new
// #define un_del(ptr) if (ptr) delete ptr; ptr = NULL;
// #define un_del_arr(ptr) if (ptr) delete[] ptr; ptr = NULL;

#define KNEW				new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define KNEW				new( __FILE__, __LINE__ )
#define KDEL				delete //(setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete

#define CLASS_TO(classname) #classname
#define SAFE_DEL(ptr)		if (ptr) KDEL ptr; ptr = NULL;
#define SAFE_DEL_ARR(ptr)	if (ptr) KDEL[] ptr; ptr = NULL;
#define SAFE_CALL(ptr,func) if (ptr) ptr->func;		

#define KZERO	0.0001f
#define KHALF	0.5f
#define KVEC	std::vector
#define KMAP	std::map
#define KLIST	std::list

typedef int				KINT;
typedef const int		KCINT;
typedef unsigned int	KUINT;
typedef size_t			KUINT64;
typedef float			KFLOAT;
typedef const float		KCFLOAT;
typedef char			KCHAR;
typedef bool			KBOOL;
typedef const char		KCCHAR;
typedef unsigned char	KBYTE;
typedef unsigned short	KWORD;
typedef long			KLONG;
typedef std::string		KSTR;
typedef void			KVOID;
typedef double			KDOUBLE;
typedef unsigned long	KDWORD;

typedef unsigned short int KINT2;

//////////////////////////////////////////////////////////////////////////
namespace Kylin
{
	class BtRTTI;
	class CSVFile;
	class MemoryArchive;
}
