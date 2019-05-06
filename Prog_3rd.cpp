#include "pch.h"
#include "omp.h"
#include <iostream>
using namespace std;

int min(int* line, int m)
{
	int min = line[0];
	for (int j = 0; j < m; j++)
	{
		if (line[j] < min)
		{
			min = line[j];
		}
	}
	return min;
}

int max(int** array, int n, int m, bool is_parallel)
{
	int* min_elements = new int[n];

	#pragma omp parallel if(is_parallel) 
	{
	#pragma omp for
		for (int i = 0; i < n; i++)
		{
			min_elements[i] = min(array[i], m);
		}
	}

	int max = min_elements[0];
	#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		if (min_elements[i] > max)
		{
			max = min_elements[i];
		}
	}
	return max;
}



int main()
{
	int n = 1000;
	int m = 1000;
	int** array = new int*[n];

	for (int i = 0; i < n; i++)
	{
		array[i] = new int[m];
		for (int j = 0; j < m; j++)
		{
			array[i][j] = 2*i + 2*j % 20000;
		}
	}

	double time_1 = omp_get_wtime();
	double answer_1 = max(array, n, m, false);
	double end_time_1 = omp_get_wtime();
	double time_2 = omp_get_wtime();
	double answer_2 = max(array, n, m, true);
	double end_time_2 = omp_get_wtime();

	cout << "Answer_1: " << answer_1 << endl;
	cout << "Time without parallel: " << (end_time_1 - time_1) * 1000 << " ms" << endl;
	cout << "Answer_2: " << answer_2 << endl;
	cout << "Time with parallel: " << (end_time_2 - time_2) * 1000 << " ms" << endl;
	return 0;
}