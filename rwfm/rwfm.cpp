#include "stdafx.h"
#include "rwfm.h"

namespace
{
	std::unordered_map<ulong_t, HANDLE> viewHandles;

	int raiseError(int * error)
	{
		if (error != nullptr) {
			*error = GetLastError();
		}
		return 0;
	}

	std::int64_t cpuFrequency()
	{
		LARGE_INTEGER qpf;
		QueryPerformanceFrequency(&qpf);
		return qpf.QuadPart / 1000;
	}
}

RWFM_API ulong_t openView(const TCHAR name[], int * error)
{
	if (name == nullptr) {
		return raiseError(error);
	}

	const auto handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);
	if (handle == NULL) {
		return raiseError(error);
	}

	const auto buffer = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (buffer == nullptr) {
		CloseHandle(handle);
		return raiseError(error);
	}

	const auto result = reinterpret_cast<ulong_t>(buffer);
	viewHandles[result] = handle;

	return result;
}

RWFM_API bool closeView(ulong_t view)
{
	auto result = false;
	const auto iterator = viewHandles.find(view);

	if (iterator != viewHandles.end()) {
		UnmapViewOfFile(reinterpret_cast<void*>(view));
		CloseHandle(iterator->second);
		viewHandles.erase(iterator);
		result = true;
	}

	return result;
}

RWFM_API void getData(ulong_t view, int position, char buffer[], int offset, int length)
{
	CopyMemory(buffer + offset, reinterpret_cast<char*>(view) + position, length);
}

RWFM_API void setData(ulong_t view, int position, char buffer[], int offset, int length)
{
	CopyMemory(reinterpret_cast<char*>(view) + position, buffer + offset, length);
}

RWFM_API int_t getAndAddInt(ulong_t view, int position, int_t delta)
{
	const auto memory = reinterpret_cast<char*>(view) + position;
	return InterlockedExchangeAdd(reinterpret_cast<uint_t*>(memory), delta);
}

RWFM_API long_t getAndAddLong(ulong_t view, int position, long_t delta)
{
	const auto memory = reinterpret_cast<char*>(view) + position;
	return InterlockedExchangeAdd64(reinterpret_cast<long_t*>(memory), delta);
}

RWFM_API int_t getInt(ulong_t view, int position)
{
	const auto memory = reinterpret_cast<char*>(view) + position;
	return *reinterpret_cast<int_t*>(memory);
}

RWFM_API void setInt(ulong_t view, int position, int value)
{
	const auto memory = reinterpret_cast<char*>(view)+position;
	*reinterpret_cast<int_t*>(memory) = value;
}

RWFM_API long_t getLong(ulong_t view, int position)
{
	const auto memory = reinterpret_cast<char*>(view) + position;
	return *reinterpret_cast<long_t*>(memory);
}

RWFM_API void setLong(ulong_t view, int position, long_t value)
{
	const auto memory = reinterpret_cast<char*>(view) + position;
	*reinterpret_cast<long_t*>(memory) = value;
}

RWFM_API long_t waitNewLong(ulong_t view, int position, long_t current, int parkMills, int timeoutMills)
{
	static const auto frequency = cpuFrequency();
	auto result = long_t(-1);
	LARGE_INTEGER qpc;

	QueryPerformanceCounter(&qpc);
	const auto delay = qpc.QuadPart + timeoutMills * frequency;
	
	while (QueryPerformanceCounter(&qpc) && delay > qpc.QuadPart) {
		const auto memory = reinterpret_cast<char*>(view) + position;
		result = *reinterpret_cast<long_t*>(memory);
		if (result != current) {
			break;
		}
		Sleep(parkMills);
	}

	return result;
}
