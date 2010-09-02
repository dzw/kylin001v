
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XHOOK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XHOOK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef XHOOK_EXPORTS
#define XHOOK_API __declspec(dllexport)
#else
#define XHOOK_API __declspec(dllimport)
#endif

// This class is exported from the xHook.dll
class XHOOK_API CXHook {
public:
	CXHook(void);
	// TODO: add your methods here.
};

extern XHOOK_API int nXHook;

XHOOK_API int fnXHook(void);

