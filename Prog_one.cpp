#include "pch.h"
#include <iostream>
#include "omp.h"
using namespace std;

int main()
{
	const int n = 99999;
	double answ = 0;
	double start = omp_get_wtime();
	for (int i = 0; i < n; i += 1)
	{
		answ = answ + 4.0 / (1 + (double(i) / n)*(double(i) / n));
	}
	answ = answ / n;
	double end = omp_get_wtime();
	cout << "Time with parallel: " << (end - start) * 1000 << endl;
	cout << "Result: " << answ << endl;
	answ = 0;
	start = omp_get_wtime();

#pragma omp parallel for 
	for (int i = 0; i < n; i += 1)
	{
		answ = answ + 4.0 / (1 + (double(i) / n)*(double(i) / n));
	}
	answ = answ / n;
	end = omp_get_wtime();
	cout << "Time with parallel: " << (end - start) * 1000 << endl;
	cout << "Result: " << answ << endl;
	answ = 0;

#pragma omp parallel reduction(+: answ)
	{
		start = omp_get_wtime();
		#pragma omp for 
		for (int i = 0; i < n; i += 1)
		{
			answ = answ + 4.0 / (1 + (double(i) / n)*(double(i) / n));
		}
		answ = answ / n;
		end = omp_get_wtime();
	}
	cout << "Time with parallel: " << (end - start) * 1000 << endl;
	cout << "Result: " << answ << endl;

	return 0;
}