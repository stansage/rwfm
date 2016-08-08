#pragma once

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
	RWFM_API void getData(int position, char buffer[], int offset, int length);	RWFM_API void setData(int position, char buffer[], int offset, int length);	RWFM_API long getAndAddLong(int position, long delta);	RWFM_API long getLong(int position);	RWFM_API long waitNewLong(int position, long current, long parkNanos, long timeoutMills);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

