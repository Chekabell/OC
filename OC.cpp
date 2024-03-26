#include <iostream>
#include <Windows.h>
#include <random>

void randomGen(int* array, const int len){
	srand(time(0));
	for (int i = 0; i < len; i++) {
		array[i] = rand();
	}
}

DWORD WINAPI bubbleSort (){

}

int main(void) {
	const int len = 100000;
	int* array = new int[len];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	randomGen(array, len);
	if (CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) std::cout << "Creating success" << std::endl;
	else { std::cout << "Creating fail" << std::endl; return 0; };


}