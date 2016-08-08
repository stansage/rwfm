// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWFM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWFM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWFM_EXPORTS
#define RWFM_API __declspec(dllexport)
#else
#define RWFM_API __declspec(dllimport)
#endif

#include <tchar.h>
#include <string>

#ifdef _UNICODE
using String = std::wstring;
#else
using String = std::wstring;
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	RWFM_API int openView(const TCHAR path[], int * error);
	RWFM_API bool selectView(int view);
	RWFM_API bool closeView();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

