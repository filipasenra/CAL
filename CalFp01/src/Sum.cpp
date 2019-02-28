/*
 * Sum.cpp
 */

#include "Sum.h"
#include "tuple"
#include <chrono>

string calcSum(int* sequence, int size)
{
	stringstream output;

	for(int i = 1; i <= size; i++)
	{
		tuple<int, int> a = minSoma(sequence, size, i);
		output << get<0>(a) << "," << get<1>(a) << ";";
	}

	return output.str();
}

tuple<int, int> minSoma(int* sequence, int size, int M) {

	int minSum = 0;
	int currentSum = 0;
	int currentMinIndex = 0;

	//Soma inicial
	for(int i = 0; i < M; i++)
		minSum += sequence[i];

	currentSum = minSum;

	//calcular a soma dos elementos seguintes
	for (int i = 1; i < size-M + 1; i++)
	{
		//temos a soma de n elementos seguidos
		//retiramos o 1o elemento e adicionamos o elemento de indice seguinte (de indice i-1+M)
		//obtemos a soma da nova sequencia
		currentSum -= sequence[i-1];
		currentSum += sequence[i - 1 + M];

		if (currentSum < minSum)
		{
			minSum = currentSum;
			currentMinIndex = i;
		}
	}

	return make_tuple(minSum, currentMinIndex);
}

