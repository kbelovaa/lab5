// spo5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

LONG a[10];

int thread1()
{
	srand(time(NULL));
	HANDLE hMutex01 = OpenMutex(SYNCHRONIZE, FALSE, TEXT("MyMutex"));
	if (hMutex01 == NULL)
		cout << "Open mutex01 failed" << GetLastError() << endl;
	for (int i = 0; i < 3; i++)
	{
		WaitForSingleObject(hMutex01, INFINITE);
		for (int j = 0; j < 10; j++)
		{
			LONG k = rand() % 300 - 150;
			InterlockedExchange(&a[j], k);
		}
		cout << "Original array:" << endl;
		for (int j = 0; j < 10; j++)
		{
			cout << a[j] << " ";
		}
		cout << endl;
		ReleaseMutex(hMutex01);
	}
	CloseHandle(hMutex01);
	return 0;
}

int thread2()
{
	HANDLE hMutex02 = OpenMutex(SYNCHRONIZE, FALSE, TEXT("MyMutex"));
	if (hMutex02 == NULL)
		cout << "Open mutex02 failed" << GetLastError() << endl;
	for (int i = 0; i < 3; i++)
	{
		WaitForSingleObject(hMutex02, INFINITE);
		for (int j = 0; j < 10; j++)
		{
			if (j % 2 == 1)
				a[j] = 0;

		}
		cout << "Modified array:" << endl;
		for (int j = 0; j < 10; j++)
		{
			cout << a[j] << "  ";
		}
		cout << endl << endl;
		ReleaseMutex(hMutex02);
	}
	CloseHandle(hMutex02);
	return 0;
}

int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("MyMutex"));
	if (hMutex == NULL)
		cout << "Create mutex failed" << GetLastError() << endl;

	HANDLE hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread1, NULL, 0, NULL);
	if (hTh01 == NULL) return GetLastError();

	HANDLE hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread2, NULL, 0, NULL);
	if (hTh02 == NULL) return GetLastError();

	cin.get();
	CloseHandle(hMutex);
	CloseHandle(hTh01);
	CloseHandle(hTh02);
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
