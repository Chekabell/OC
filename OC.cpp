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
	randomGen(array, len);


}