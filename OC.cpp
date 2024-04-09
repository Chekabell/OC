#include <iostream>
#include <Windows.h>
#include <strsafe.h>

#define BUFF_SIZE 2048

void ErrorExit(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintfW((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}


int main(void) {
	HANDLE handleChildRd;
	HANDLE handleParentWr;

	SECURITY_ATTRIBUTES securityAttrib;

	securityAttrib.bInheritHandle = TRUE;
	securityAttrib.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&handleChildRd, &handleParentWr, &securityAttrib, NULL)) {
		std::cout << "Failed creating anonym pipe." << std::endl;
	}

	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startInfo, sizeof(startInfo));
	ZeroMemory(&processInfo, sizeof(processInfo));

	startInfo.hStdInput = handleChildRd;
	startInfo.dwFlags |= STARTF_USESTDHANDLES;

	std::wstring CommandLine(L"C:\\Users\\User\\source\\repos\\Sub OS\\x64\\Release\\Sub OS.exe");
	LPWSTR lpwCmdLine = &CommandLine[0];

	if (!CreateProcess(NULL, lpwCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &processInfo)) {
		std::cout << "Failed creating sub process" << std::endl;
	}

	DWORD dwWrite;
	CHAR buffer[BUFF_SIZE] = "63728" ;
	
	if (!WriteFile(handleParentWr, buffer, (DWORD)sizeof(buffer), &dwWrite, NULL)) {
		std::cout << "Failed writing to pipe!" << std::endl;
	}

	if (!CloseHandle(handleParentWr)) {
		std::cout << "Failed closing handleParent wr!" << std::endl;
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return 0;
}