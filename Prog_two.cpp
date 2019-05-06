#include "pch.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <string>
using namespace std;

double a_b (vector<double> a, vector<double> b, int n, int paral)
{

	double sum = 0;

	if (paral == 0) 
	{
		for (int i = 0; i < n; i++)
			sum += a[i] * b[i];

	}

	if (paral == 1) 
	{
	#pragma omp parallel for reduction (+:sum)
		for (int i = 0; i < n; i++)
			sum += a[i] * b[i];
	}

	return sum;
}


int main()
{
	int n = 10000000;
	srand(time(NULL));

	vector<double> a(n, 1 + rand() % 5);
	vector<double> b(n, 1 + rand() % 5);

	string hint[2] = 
	{
			"Time without parallel: ",
			"Time with parallel: ",
	};

	for (int i = 0; i < 2; i++) 
	{
		double start = omp_get_wtime();
		float answ = a_b(a, b, n, i);
		double end = omp_get_wtime();
		double seconds = (double)(end - start);
		cout << "Result: " << answ << endl;
		cout << seconds << " sec " << hint[i] << endl;
	}

	return 0;
}