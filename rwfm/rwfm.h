#pragma once

#ifdef RWFM_EXPORTS
#define RWFM_API __declspec(dllexport)
#else
#define RWFM_API __declspec(dllimport)
#endif

#include <tchar.h>
#include <cstddef>
#include <cinttypes>

typedef std::int32_t int_t;
typedef std::uint32_t uint_t;
typedef std::int64_t long_t;
typedef std::uint64_t ulong_t;


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	RWFM_API ulong_t openView(const TCHAR path[], int * error);
	RWFM_API bool closeView(ulong_t view);

	RWFM_API void getData(ulong_t view, int position, char buffer[], int offset, int length);
	RWFM_API void setData(ulong_t view, int position, char buffer[], int offset, int length);

	RWFM_API int_t getAndAddInt(ulong_t view, int position, int_t delta);
	RWFM_API long_t getAndAddLong(ulong_t view, int position, long_t delta);

	RWFM_API int_t getInt(ulong_t view, int position);
	RWFM_API void setInt(ulong_t view, int position, int_t value);

	RWFM_API long_t getLong(ulong_t view, int position);
	RWFM_API void setLong(ulong_t view, int position, long_t value);
	
	RWFM_API long_t waitNewLong(ulong_t view, int position, long_t current, int parkNanos, int timeoutMills);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

