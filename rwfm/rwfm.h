#pragma once

#ifdef RWFM_EXPORTS
#define RWFM_API __declspec(dllexport)
#else
#define RWFM_API __declspec(dllimport)
#endif

#include <tchar.h>
#include <cstddef>
#include <cinttypes>

typedef std::size_t view_t;
typedef std::uint64_t long_t;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	RWFM_API view_t openView(const TCHAR path[], int * error);
	RWFM_API bool selectView(view_t view);
	RWFM_API bool closeView();
	RWFM_API void getData(int position, char buffer[], int offset, int length);
	RWFM_API void setData(int position, char buffer[], int offset, int length);
	RWFM_API long_t getAndAddLong(int position, long_t delta);
	RWFM_API long_t getLong(int position);
	RWFM_API long_t waitNewLong(int position, long_t current, int parkNanos, int timeoutMills);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

