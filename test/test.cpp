// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int error[] = { 0 };
	const auto name0 = _T("Local\\testfile0");
	const auto handle0 = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 1024, name0);
	if (handle0 == nullptr) {
		std::cout << "Create_0: " << GetLastError() << std::endl;
	} else {
		const auto view0 = openView(name0, error);

		if (view0 == 0) {
			std::cout << "Error0: " << error[0] << std::endl;
		} else {
			const auto name1 = _T("Local\\testfile1");
			const auto handle1 = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 1024, name1);
			
			if (handle0 == nullptr) {
				std::cout << "Create_1: " << GetLastError() << std::endl;
			} else {
				const auto view1 = openView(name1, error);

				if (view1 == 0) {
					std::cout << "Error1: " << error[0] << std::endl;
				} else {
					selectView(view0);
					closeView();
					selectView(view1);
					closeView();
				}
				CloseHandle(handle1);
			}
		}

		CloseHandle(handle0);
	}

	return 0;
}

