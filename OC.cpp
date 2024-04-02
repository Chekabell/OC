#include <iostream>
#include <Windows.h>

struct ArrInfo {
	int* arr;
	int len;
};

enum Priority { Standard, First, Second };

void randomGen(int* array, const int len){
	srand(NULL);
	for (int i = 0; i < len; i++) {
		array[i] = rand()%len;
	}
}

DWORD WINAPI bubbleSort(LPVOID lpParam) {
	ArrInfo a = *((ArrInfo*)lpParam);
	int len = a.len;
	int* tmpArray = new int[len];

	for (int i = 0; i < len - 1; i++) {
		tmpArray[i] = a.arr[i];
	}

	int tmp;
	bool swapped;
	for (int i = 0; i < len - 1; i++) {
		swapped = false;
		for (int j = 0; j < len - i - 1; j++) {
			if (tmpArray[j] > tmpArray[j + 1]) {
				tmp = tmpArray[j];
				tmpArray[j] = tmpArray[j + 1];
				tmpArray[j + 1] = tmp;
				swapped = true;
			}
		}
		if (swapped == false) {
			break;
		}
	}

	return NULL;
}

void quickSort(int* arr, int len) {
	int i = 0;
	int j = len - 1;

	int mid = arr[len / 2];

	do {
		while (arr[i] < mid) {
			i++;
		}
		while (arr[j] > mid) {
			j--;
		}

		if (i <= j) {
			int tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;

			i++;
			j--;
		}
	} while (i <= j);


	if (j > 0) {
		quickSort(arr, j + 1);
	}
	if (i < len) {
		quickSort(&arr[i], len - i);
	}
}

DWORD WINAPI quickSort(LPVOID lpParam){
	ArrInfo a = *((ArrInfo*)lpParam);
	int len = a.len;
	int* tmpArray = new int[len];

	for (int i = 0; i < len - 1; i++) {
		tmpArray[i] = a.arr[i];
	}
	//Указатели в начало и в конец массива
	int i = 0;
	int j = len - 1;

	//Центральный элемент массива
	int mid = tmpArray[len / 2];

	//Делим массив
	do {
		//Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
		//В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
		while (tmpArray[i] < mid) {
			i++;
		}
		//В правой части пропускаем элементы, которые больше центрального
		while (tmpArray[j] > mid) {
			j--;
		}

		//Меняем элементы местами
		if (i <= j) {
			int tmp = tmpArray[i];
			tmpArray[i] = tmpArray[j];
			tmpArray[j] = tmp;

			i++;
			j--;
		}
	} while (i <= j);


	//Рекурсивные вызовы, если осталось, что сортировать
	if (j > 0) {
		//"Левый кусок"
		quickSort(tmpArray, j + 1);
	}
	if (i < len) {
		//"Правый кусок"
		quickSort(&tmpArray[i], len - i);
	}
	return NULL;
}

static void checkThread(Priority p, const int len) {
	int aPriority, bPriority;

	switch (p) {
	case Priority::Standard:
		aPriority = THREAD_PRIORITY_NORMAL;
		bPriority = THREAD_PRIORITY_NORMAL;
		break;
	case Priority::First:
		aPriority = THREAD_PRIORITY_HIGHEST;
		bPriority = THREAD_PRIORITY_LOWEST;
		break;
	case Priority::Second:
		aPriority = THREAD_PRIORITY_LOWEST;
		bPriority = THREAD_PRIORITY_HIGHEST;
		break;
	};

	FILETIME fl1, fl2, fl3, fl4, fl5, fl6, fl7, fl8;
	SYSTEMTIME st1, st2, st3, st4, st5, st6, st7, st8;

	ArrInfo arrInf1{ new int[len],len }, arrInf2{ new int[len],len };

	randomGen(arrInf1.arr, len);
	randomGen(arrInf2.arr, len);

	HANDLE additTh1 = CreateThread(NULL, 0, &bubbleSort, &arrInf1, CREATE_SUSPENDED, NULL);
	SetThreadPriority(additTh1, aPriority);

	HANDLE additTh2 = CreateThread(NULL, 0, &quickSort, &arrInf2, CREATE_SUSPENDED, NULL);
	SetThreadPriority(additTh2, bPriority);

	SetProcessPriorityBoost(additTh1, TRUE);
	SetProcessPriorityBoost(additTh2, TRUE);

	ResumeThread(additTh1);
	ResumeThread(additTh2);

	WaitForSingleObject(additTh1, INFINITE);
	WaitForSingleObject(additTh2, INFINITE);

	GetThreadTimes(additTh1, &fl1, &fl2, &fl3, &fl4);
	GetThreadTimes(additTh2, &fl5, &fl6, &fl7, &fl8);

	FileTimeToSystemTime(&fl1, &st1);
	FileTimeToSystemTime(&fl2, &st2);
	FileTimeToSystemTime(&fl3, &st3);
	FileTimeToSystemTime(&fl4, &st4);
	FileTimeToSystemTime(&fl5, &st5);
	FileTimeToSystemTime(&fl6, &st6);
	FileTimeToSystemTime(&fl7, &st7);
	FileTimeToSystemTime(&fl8, &st8);

	std::cout << "BUBBLE SORT: " << std::endl;
	std::cout << "Creation time: " << st1.wMinute << ":" << st1.wSecond << ":" << st1.wMilliseconds << std::endl;
	std::cout << "Exit time: "  << st2.wMinute << ":" << st2.wSecond << ":" << st2.wMilliseconds << std::endl;
	std::cout << "TIME LIVE: " << (abs(st2.wMinute*60000)+abs(st2.wSecond * 1000) + abs(st2.wMilliseconds)) - (abs(st1.wMinute * 60000)+abs(st1.wSecond * 1000) + abs(st1.wMilliseconds)) << " ms" << std::endl;
	std::cout << "Core time: "  << st3.wMinute << ":" << st3.wSecond << ":" << st3.wMilliseconds << std::endl;
	std::cout << "User time: "  << st4.wMinute << ":" << st4.wSecond << ":" << st4.wMilliseconds << std::endl;
	

	std::cout << "QUICK SORT: " << std::endl;
	std::cout << "Creation time: " << st5.wMinute << ":" << st5.wSecond << ":" << st5.wMilliseconds << std::endl;
	std::cout << "Exit time: " << st6.wMinute << ":" << st6.wSecond << ":" << st6.wMilliseconds << std::endl;
	std::cout << "TIME LIVE: " << (abs(st6.wMinute * 60000)+abs(st6.wSecond * 1000) + abs(st6.wMilliseconds)) - (abs(st5.wMinute * 60000)+abs(st5.wSecond * 1000) + abs(st5.wMilliseconds)) << " ms" << std::endl;
	std::cout << "Core time: " << st7.wMinute << ":" << st7.wSecond << ":" << st7.wMilliseconds << std::endl;
	std::cout << "User time: " << st8.wMinute << ":" << st8.wSecond << ":" << st8.wMilliseconds << std::endl;

	TerminateThread(additTh1, 0);
	TerminateThread(additTh2, 0);

	CloseHandle(additTh1);
	CloseHandle(additTh2);
}

int main(void) {
	const int len = 1000;
	
	std::cout << "Bubble - standard || Quick - standard" << std::endl;
	checkThread(Priority::Standard, len);
	std::cout << std::endl << "Bubble - highest || Quick - lowest:" << std::endl;
	checkThread(Priority::First, len);
	std::cout << std::endl << "Bubble - lowest || Quick - highest:" << std::endl;
	checkThread(Priority::Second, len);

	return 0;
}