/*
 * Change.cpp
 */

/**
 * @brief Apliquei a fórmula apresentada pelo professor para o caso da mochila
 * (excepto que neste caso, o vantajoso é  ter menos moedas)
 * assumi que o minCoins era algo equivalente ao vetor cost
 * e que lastCoin era algo equivalente ao vetor best
 * e que coinValues era equivalente a size
 * e que val era 1 para todas as moedas
 */

#include "Change.h"
#include <vector>

string calcChange(int m, int numCoins, int *coinValues) {

	//Caso nenhumas moedas terem sido passadas ou o troco for nulo
	if (numCoins == 0 || m == 0)
		return "";

	//Verifica se é possível fazer troco com as moedas disponíveis
	if (minCoins(numCoins - 1, m, coinValues) == 0) //não é possível fazer o troco
		return "-";

	stringstream output;

	//Ultima moeda a ser utilizada para que o troco tenha o menor numero de moedas possivel
	int mCoin = lastCoin(numCoins - 1, m, coinValues);

	while (mCoin != 0) {
		//montante restante tirando a ultima moeda utilizada para fazer o troco
		m -= mCoin;
		output << mCoin << ";";
		mCoin = lastCoin(numCoins - 1, m, coinValues);
	}

	return output.str();

}

/**
 *
 * @param valor do troco
 * @param indice da moeda mais alta disponivel
 * @param valores das moedas
 *
 * @return numero minimo de moedas
 */
int minCoins(int i, int k, int *coinValues) {

	//impossivel: k negativo
	if (k < 0)
		return -1;

	//Caso base
	if (k == 0)
		return 0;

	//Caso Recursivo
	//(se só tiver disponível a moeda de menor valor,
	//esta será a moeda de valor mais alto no troco)
	if (i == 0
			|| (coinValues[i] <= k
					&& (1 + minCoins(i, k - coinValues[i], coinValues)
							< minCoins(i - 1, k, coinValues)))) {
		return 1 + minCoins(i, k - coinValues[i], coinValues);
	} else {
		return minCoins(i - 1, k, coinValues);
	}
}

/**
 *
 * @param valor do troco
 * @param indice da moeda mais alta disponivel
 * @param valores das moedas
 *
 * @return valor da utlima moeda a usar para fazer o troco com menos moedas
 */
int lastCoin(int i, int k, int *coinValues) {

	//Caso Base
	if (k == 0 || i == -1)
		return 0;

	//Caso Recursivo
	//(se só tiver disponível a moeda de menor valor,
	//esta será a moeda de valor mais alto no troco)
	if (i == 0
			|| (coinValues[i] <= k
					&& (1 + minCoins(i, k - coinValues[i], coinValues)
							< minCoins(i - 1, k, coinValues)))) {
		return coinValues[i];
	} else {
		return lastCoin(i - 1, k, coinValues);
	}
}
