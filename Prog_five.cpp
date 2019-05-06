#include "pch.h"
#include "omp.h"
#include <iostream>
using namespace std;


int* row_product(int** a, int* b, int m, int n, unsigned int threads_num)
{
	int* result = new int[m];
	for (int i = 0; i < m; i++)
	{
		result[i] = 0;
	}
	#pragma omp parallel shared(a, b, result) num_threads(threads_num)
	{
	#pragma omp for nowait
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result[i] += a[i][j] * b[j];
			}
		}
	}
	return result;
}

int* column_product(int** a, int* b, int m, int n, unsigned int threads_num)
{
	int* result = new int[m];
	for (int i = 0; i < m; i++)
	{
		result[i] = 0;
	}
	#pragma omp parallel shared(a, b, result) num_threads(threads_num)
	{
	#pragma omp for
		for (int j = 0; j < n; j++)
		{
			for (int i = 0; i < m; i++)
			{
				result[i] += a[i][j] * b[j];
			}
		}
	}
	return result;
}

int* block_product(int** a, int* b, int m, int n, unsigned int threads_num)
{
	int* result = new int[m];
	for (int i = 0; i < m; i++)
	{
		result[i] = 0;
	}
	#pragma omp parallel shared(a, b, result) num_threads(threads_num)
	{
		int s = omp_get_num_threads();
		int q = s;
		int block_height = m / s;
		int block_width = n / q;

	#pragma omp for
		for (int block_count = 0; block_count < s*q; block_count++)
		{
			int i = block_count / q;
			int j = block_count % q;
			for (int k = i * block_height; k < (i + 1) * block_height; k++)
				for (int l = j * block_width; l < (j + 1) * block_width; l++)
					result[k] += a[k][l] * b[l];
		}
	}
	return result;
}




int main()
{
	int max_threads_number = 8;
	int max_dim = 100000;

	for (int threads_number = 1; threads_number < max_threads_number + 1; threads_number *= 2)
	{
		cout << "Threads number: " << threads_number << endl;
		for (int dim = 10; dim < max_dim; dim *= 10)
		{
			cout << "Dim: " << dim << endl;
			int m = dim;
			int n = dim;
			int** a = new int*[m];
			for (int i = 0; i < m; i++)
			{
				a[i] = new int[n];
				for (int j = 0; j < n; j++)
				{
					a[i][j] = rand() % 10;
				}
			}
			int* b = new int[n];
			for (int i = 0; i < n; i++)
			{
				b[i] = rand() % 10;
			}

			double start1 = omp_get_wtime();
			int* result1 = row_product(a, b, m, n, threads_number);
			double end1 = omp_get_wtime();

			cout << "Time row_product: " << (end1 - start1) * 1000 << " ms" << endl;

			double start2 = omp_get_wtime();
			int* result2 = column_product(a, b, m, n, threads_number);
			double end2 = omp_get_wtime();

			cout << "Time column_product: " << (end2 - start2) * 1000 << " ms" << endl;

			double start3 = omp_get_wtime();
			int* result3 = block_product(a, b, m, n, threads_number);
			double end3 = omp_get_wtime();

			cout << "Time block_product: " << (end3 - start3) * 1000 << " ms" << endl;

		}
	}
	return 0;
}