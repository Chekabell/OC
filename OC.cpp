#include <iostream>
#include <Windows.h>

int main(void) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	FILETIME fl1, fl2, fl3, fl4;
	SYSTEMTIME st1, st2, st3, st4;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) std::cout << "Creating success" << std::endl;
	else { std::cout << "Creating fail" << std::endl; return 0; };

	WaitForSingleObject(pi.hProcess, INFINITE);
	GetProcessTimes(pi.hProcess, &fl1, &fl2, &fl3, &fl4);

	FileTimeToSystemTime(&fl1, &st1);
	FileTimeToSystemTime(&fl2, &st2);
	FileTimeToSystemTime(&fl3, &st3);
	FileTimeToSystemTime(&fl4, &st4);

	std::cout << "Time start: " << st1.wMinute << ":" << st1.wSecond << ":" << st1.wMilliseconds << std::endl;
	std::cout << "Time end: " << st2.wMinute << ":" << st2.wSecond << ":" << st2.wMilliseconds << std::endl;
	std::cout << "Time in core mode: " << st3.wMinute << ":" << st3.wSecond << ":" << st3.wMilliseconds << std::endl;
	std::cout << "Time in user mode: " << st4.wMinute << ":" << st4.wSecond << ":" << st4.wMilliseconds << std::endl;
	std::cout << "Handle: " << pi.hProcess << std::endl;

	CloseHandle(pi.hProcess);

	return 0;
}