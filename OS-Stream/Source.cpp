#include <windows.h>
#include <iostream>
volatile double n;
int save_index_big = 0;
int save_index_small = 0;
double save_average = 0;

struct array {
	array(double* arr, double size) : arr(arr), size(size) {};
	double* arr;
	double min = 0;
	double max = 0;
	double average = 0;
	double count = 0;
	double size;
};

DWORD WINAPI min_max(LPVOID arguments) {
	array* Array = (array*)arguments;
	Array->max = Array->min = Array->arr[0];
	for (int i = 0; i < Array->size; i++)
	{
		if (Array->arr[i] > Array->max)
		{
			Array->max = Array->arr[i];
			save_index_big = i;
		}
		if (Array->arr[i] < Array->min)
		{
			Array->min = Array->arr[i];
			save_index_small = i;
		}
		Sleep(7);
	}
	std::cout << "the smallest element of the array = " << Array->min << std::endl;
	std::cout << "the biggest element of the array = " << Array->max << std::endl;
	return 0;
}

DWORD WINAPI average(LPVOID arguments) {
	array* Array = (array*)arguments;
	for (int i = 0; i < Array->size; i++)
	{
		Array->average += Array->arr[i];
		Array->count++;
		Sleep(12);
	}
	Array->average = Array->average / Array->count;
	save_average = Array->average;
	std::cout << "average = " << Array->average << std::endl;
	return 0;
}

int main()
{
	int size_array;
	std::cout << "size of array ";
	std::cin >> size_array;
	double* ar = new double[size_array];
	std::cout << "enter the array elements " << std::endl;
	for (int i = 0; i < size_array; i++)
	{
		std::cin >> ar[i];
	}

	HANDLE hThread;
	DWORD IDThread;
	array* array_struct = new array(ar, size_array);
	hThread = CreateThread(NULL, 0, min_max, (void*)array_struct, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	HANDLE hThread_average;
	hThread_average = CreateThread(NULL, 0, average, (void*)array_struct, 0, &IDThread);
	if (hThread_average == NULL)
		return GetLastError();

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hThread_average, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hThread_average);
	ar[save_index_big] = save_average;
	ar[save_index_small] = save_average;
	for (int i = 0; i < size_array; i++)
	{
		std::cout << ar[i] << " ";
	}
	return 0;
}