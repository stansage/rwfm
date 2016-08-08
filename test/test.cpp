// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int error[] = { 0 };
	const auto name0 = _T("Local\\testfile0");
	const auto handle0 = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 1024, name0);

	if (handle0 == nullptr) {
		std::cerr << "Create_0: " << GetLastError() << std::endl;
	} else {
		const auto view0 = openView(name0, error);

		if (view0 == 0) {
			std::cerr << "Error0: " << error[0] << std::endl;
		} else {
			const auto name1 = _T("Local\\testfile1");
			const auto handle1 = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 1024, name1);

			if (handle0 == nullptr) {
				std::cerr << "Create_1: " << GetLastError() << std::endl;
			} else {
				const auto view1 = openView(name1, error);

				if (view1 == 0) {
					std::cerr << "Error1: " << error[0] << std::endl;
				} else {
					selectView(view0);
					closeView();
					selectView(view1);
					closeView();
				}
				CloseHandle(handle1);
			}
		}

		const auto view = openView(name0, error);

		if (view == 0) {
			std::cerr << "Error: " << error[0] << std::endl;
		} else {
			char buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			setData(123, buffer, 3, 4);
			ZeroMemory(buffer, 10);

			getData(123, buffer, 0, 4);
			if (buffer[0] != 3 || buffer[1] != 4 || buffer[2] != 5 || buffer[3] != 6) {
				std::cerr << "Fail: " << int(buffer[0]) << int(buffer[1]) << int(buffer[2]) << int(buffer[3]) << std::endl;
			}

			const auto current = getLong(123);
			auto value = waitNewLong(123, current, 100, 1000);

			if (value != current) {
				std::cerr << "Fail: " << value << " != " << current << std::endl;
			} else {
				std::thread thread([] {
					Sleep(1300);
					char buffer[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
					setData(123, buffer, 4, 3);
				});

				value = waitNewLong(123, current, 100, 3000);
				if (value == current) {
					std::cerr << "Fail: " << value << " == " << current << std::endl;
				}
			}
		}

		CloseHandle(handle0);
	}

	return 0;
}

