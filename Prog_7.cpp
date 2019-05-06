#include "pch.h"
#include "omp.h"
#include <string.h>
#include <iostream>
using namespace std;

int function(const char* string, const char* substring, bool is_parallel)
{
	int result = -1;
	#pragma omp parallel shared(string, substring) if(is_parallel)
	{
		#pragma omp for
		for (int i = 0; i < strlen(string) - strlen(substring) + 1; i++)
		{
			int matches_count = 0;
			for (int j = 0; j < strlen(substring); j++)
			{
				if (string[i + j] != substring[j])
					break;
				matches_count++;
			}
			if (matches_count == strlen(substring))
			{
				result = i;
			}
		}
	}
	return result;
}

int main()
{
	auto string = "In association football, a substitute is a player who is brought on to the pitch during a match in exchange for an existing player. Substitutions are generally made to replace a player who has become tired or injured, or who is performing poorly, or for tactical reasons (such as bringing a striker on in place of a defender). Unlike some sports (such as American football, ice hockey or kabaddi), a player who has been substituted during a match may take no further part in it.";
	auto substring = "football";

	double start1 = omp_get_wtime();
	int result1 = function(string, substring, false);
	double end1 = omp_get_wtime();
	double start2 = omp_get_wtime();
	int result2 = function(string, substring, true);
	double end2 = omp_get_wtime();

	cout << "Answer_1: " << result1 << endl;
	cout << "Time with parallel: " << (end1 - start1) * 1000 << " ms" << endl;
	cout << "Answer_2: " << result2 << endl;
	cout << "Time without parallel: " << (end2 - start2) * 1000 << " ms" << endl;
	return 0;
}