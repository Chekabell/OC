/*------------------------------------------------------
������ ������ ������������� "�������������-�����������".
�������� ������.
--------------------------------------------------------*/
#include <Windows.h>
#include <iostream>
#include "buffer.h"
#define cProducers 3 /*���������� ��������������*/
#define cConsumers 5 /*���������� ������������*/
#define BufferSize 5 /*������ ������*/

int cOperations = 1000; /*���������� �������� ��� �������*/

DWORD __stdcall getkey(void* b) {
	cin.get();
	return cOperations = 0;
}

/*��������� � ��������� ������ �������������� ������ � ������� 
������ � ������ ���� ���������� ����, ������������
���������������� ��������:
- ������ "��������������" � "������������";
- � ���� ������� ������� (�������� �������� �������������
� ��.);
- � ���������� ������������ (���������� ���������� � ��.).*/

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