#include "stdafx.h"
#include "rwfm.h"

int selectedView = 0;
std::unordered_map<int, std::pair<LPVOID, HANDLE>> views;
std::random_device random;
std::default_random_engine rengine(random());
std::uniform_int_distribution<int> rdistribution(1, 2147483647);

int raiseError(int * error)
{
	if (error != nullptr) {
		*error = GetLastError();
	}
	return 0;
}

RWFM_API int openView(const TCHAR name[], int * error)
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

	do {
		selectedView = rdistribution(rengine);
	} while (views.find(selectedView) != views.end());
	
	views[selectedView] = std::make_pair(buffer, handle);
	
	return selectedView;
}

RWFM_API bool selectView(int view)
{
	if (views.find(view) == views.end()) {
		return false;
	}

	selectedView = view;
	return true;
}

RWFM_API bool closeView()
{
	const auto iterator = views.find(selectedView);
	if (iterator == views.end()) {
		return false;
	}

	UnmapViewOfFile(iterator->second.first);
	CloseHandle(iterator->second.second);
	views.erase(iterator);
	selectedView = 0;
	return true;
}