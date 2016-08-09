#include "stdafx.h"
#include "rwfm.h"
#include "precisetimer.hpp"

char * selectedView = 0;
std::unordered_map<view_t, HANDLE> viewHandles;

int raiseError(int * error)
{
	if (error != nullptr) {
		*error = GetLastError();
	}
	return 0;
}

long * getLongPtr(int position)
{
    return reinterpret_cast<long*>(selectedView + position);
}

RWFM_API view_t openView(const TCHAR name[], int * error)
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

        const auto result = reinterpret_cast<view_t>(buffer);
        selectedView = reinterpret_cast<char*>(result);
        viewHandles[result] = handle;

        return result;
}

RWFM_API bool selectView(view_t view)
{
        auto result = false;
        const auto iterator = viewHandles.find(view);

        if (iterator != viewHandles.end()) {
                selectedView = reinterpret_cast<char*>(view);
                result = true;
        }

        return result;
}

RWFM_API bool closeView()
{
        auto result = false;
        const auto iterator = viewHandles.find(reinterpret_cast<view_t>(selectedView));

        if (iterator != viewHandles.end()) {
                UnmapViewOfFile(selectedView);
                CloseHandle(iterator->second);
                viewHandles.erase(iterator);
                selectedView = nullptr;
                result = true;
        }

        return result;
}

RWFM_API void getData(int position, char buffer[], int offset, int length)
{
        CopyMemory(buffer + offset, selectedView + position, length);
}

RWFM_API void setData(int position, char buffer[], int offset, int length)
{
        CopyMemory(selectedView + position, buffer + offset, length);
}

RWFM_API long getAndAddLong(int position, long delta)
{
        return InterlockedExchangeAdd(getLongPtr(position), delta);
}

RWFM_API long getLong(int position)
{
        return *getLongPtr(position);
}

RWFM_API long waitNewLong(int position, long current, long parkMills, long timeoutMills)
{
        auto result = -1l;
        PreciseTimer timer;

        while (timer.elapsed() < timeoutMills) {
                result = getLong(position);
                if (result != current) {
                        break;
                }
                timer.wait(parkMills);
        }

        return result;
}
