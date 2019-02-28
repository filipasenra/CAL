/*
 * Partioning.cpp
 */

#include "Partitioning.h"

int s_recursive(int n, int k) {
	//Caso base
	if (k == 1 || k == n)
		return 1;

	//Caso recursivo
	return s_recursive(n - 1, k - 1) + k * s_recursive(n - 1, k);
}

int s_dynamic(int n, int k) {
	//vamos guardar as linhas (n-1) porque queremos guardar o valor de j
	//(sera necessario para os calculos)
	vector<int> vec(k, 1); //encher com 1's pois o caso base é 1

	//teremos que repetir n-k pois serao o numero de linhas
	//necessario calcular até chegar ao valor desejado
	//reparar no esquema apresentado nos slides da teorica
	for (int i = 1; i <= (n - k); i++) {

		//realizar os calculos para a ultima linha de n's efetuados
		for (int j = 1; j < k; j++) {

			//formula apresentada no enunciado
			vec[j] = vec[j - 1] + (j + 1) * vec[j];
		}
	}

	//retornar o ultimo elemento do vetor
	return vec[vec.size() - 1];
}

int b_recursive(int n) {
	if (n < 1)
		return -1;

	int sum = 0;

	for (int i = 1; i <= n; i++)
		sum += s_recursive(n, i);

	return sum;
}

int b_dynamic(int n) {

	if (n < 1)
		return -1;

	int sum = 0;

	for (int i = 1; i <= n; i++)
		sum += s_dynamic(n, i);

	return sum;

}

