#include <iostream>
#include <Windows.h>

#define BUFF_SIZE 2048

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

	std::wstring CommandLine(L"C:\\WINDOWS\\system32\\notepad.exe");
	LPWSTR lpwCmdLine = &CommandLine[0];

	if (!CreateProcessW(NULL, lpwCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,&startInfo, &processInfo)) {
		std::cout << "Failed creating sub process" << std::endl;
	}

	DWORD dwWritten;
	CHAR buffer[BUFF_SIZE] = "1623623" ;

	for (int i = 0; buffer[i] != '\0'; i++) {
		std::cout << ((int)buffer[i] - '0') << " ";
	}

	if (!WriteFile(handleParentWr, buffer, (DWORD)sizeof(buffer), &dwWritten, NULL)) {
		std::cout << "Failed writing to pipe!" << std::endl;
	}
	
	std::cout << "\n\n" << *((int*)&dwWritten);

	CloseHandle(handleParentWr);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return 0;
}