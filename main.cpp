/*------------------------------------------------------
Модель задачи синхронизации "Производители-потребители".
Основной модуль.
--------------------------------------------------------*/
#include <Windows.h>
#include <iostream>
#include "buffer.h"
#define cProducers 3 /*Количество производителей*/
#define cConsumers 5 /*Количество потребителей*/
#define BufferSize 5 /*Размер буфера*/

int cOperations = 1000; /*Количество операций над буфером*/

DWORD __stdcall getkey(void* b) {
	cin.get();
	return cOperations = 0;
}

/*Изменения в программе должны осуществляться только в главном 
модуле и только путём добавления кода, позволяющего
синхронизировать действия:
- внутри "производителей" и "потребителей";
- в теле главной функции (создание объектов синхронизации
и пр.);
- в глобальном пространстве (объявление переменных и пр.).*/

DWORD __stdcall producer(void* b) {
	while (cOperations-- > 0) {
		int item = rand();
		((Buffer*)b)->PutItem(item);
		Sleep(500 + rand() % 100);
	}
	return 0;
}

DWORD __stdcall consumer(void* b) {
	while (cOperations-- > 0) {
		std::cout << ((Buffer*)b)->GetItem() << std::endl;
		Sleep(500 + rand() % 100);
	}
	return 0;
}