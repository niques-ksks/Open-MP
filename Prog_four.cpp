#include "pch.h"
#include "omp.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
using namespace std;

int** mult(int** a, int** b, int m, int n, int k, bool is_parallel)
{
	int** result = new int*[m];
	for (int i = 0; i < m; i++)
	{
		result[i] = new int[k];
	}
	#pragma omp parallel shared(a, b) if(is_parallel)
	{
		for (int i = 0; i < m; i++)
		{
	#pragma omp for nowait  
			for (int j = 0; j < k; j++)
			{
				result[i][j] = 0;
				for (int s = 0; s < n; s++)
				{
					result[i][j] += a[i][s] * b[s][j];
				}
			}
		}
	}
	return result;
}

int main()
{
	int m = 100;
	int n = 1000;
	int k = 100;

	int** a = new int*[m];
	for (int i = 0; i < m; i++)
	{
		a[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			a[i][j] = std::rand() % 10;
		}
	}

	int** b = new int*[n];
	for (int i = 0; i < n; i++)
	{
		b[i] = new int[k];
		for (int j = 0; j < k; j++)
		{
			b[i][j] = std::rand() % 10;
		}
	}

	auto time_1 = omp_get_wtime();
	int** answer_1 = mult(a, b, m, n, k, false);
	auto end_time_1 = omp_get_wtime();
	auto time_2 = omp_get_wtime();
	int** answer_2 = mult(a, b, m, n, k, true);
	auto end_time_2 = omp_get_wtime();

	cout << "Time without parallel: " << (end_time_1 - time_1)*1000 << " ms" << std::endl;
	cout << "Time with parallel: " << (end_time_2 - time_2)*1000 << " ms" << std::endl;

	return 0;
}