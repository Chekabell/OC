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

int cOperations = 10; /*Количество операций над буфером*/

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

HANDLE mutex;
HANDLE semaphoreForProducer;
HANDLE semaphoreForConsumer;

DWORD __stdcall producer(void* b) {
	while (cOperations-- > 0) {
		WaitForSingleObject(semaphoreForProducer, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		int item = rand();
		((Buffer*)b)->PutItem(item);
		std::cout << "Put: " << item << std::endl;
		Sleep(500 + rand() % 100);
		ReleaseSemaphore(semaphoreForConsumer, 1, 0);
		ReleaseMutex(mutex);
	}
	return 0;
}

DWORD __stdcall consumer(void* b) {
	while (cOperations-- > 0) {
		WaitForSingleObject(semaphoreForConsumer, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		std::cout << "Get: " << ((Buffer*)b)->GetItem() << std::endl;
		Sleep(500 + rand() % 100);
		ReleaseSemaphore(semaphoreForProducer, 1, 0);
		ReleaseMutex(mutex);
	}
	return 0;
}

int main(void) {
	srand(time(NULL));
	mutex = CreateMutexW(0, 0, 0);
	semaphoreForProducer = CreateSemaphoreW(0, BufferSize, BufferSize, 0);
	semaphoreForConsumer = CreateSemaphoreW(0, 0, BufferSize, 0);
	Buffer* Buf = Buffer::CreateBuffer(BufferSize); /*Создание буфера*/
	HANDLE hThreads[cProducers + cConsumers];
	/*Вспомогательный поток, ожидающий нажатия клавиши*/
	CreateThread(0, 0, getkey, 0, 0, 0);

	for (int i = 0; i < cProducers; i++)
		/*Создание потоков-производителей*/
		hThreads[i] = CreateThread(0, 0, producer, Buf, 0, 0);
	for (int i = cProducers; i < cProducers + cConsumers; i++)
		/*Создание потоков-потребителей*/
		hThreads[i] = CreateThread(0, 0, consumer, Buf, 0, 0);

	WaitForMultipleObjects(cProducers + cConsumers, hThreads, true, INFINITE);

	cin.get();
	return 0;
}