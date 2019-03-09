/*
 * labirinth.cpp
 */

#include "Labirinth.h"

#include <iostream>
using namespace std;

Labirinth::Labirinth(int values[10][10]) {

	initializeVisited();

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			labirinth[i][j] = values[i][j];
}

void Labirinth::initializeVisited() {
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			visited[i][j] = false;
}

void Labirinth::printLabirinth() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
			cout << labirinth[i][j] << " ";

		cout << endl;
	}
}

/**
 * O meu raciocinio foi:
 * 1. avança (recurvivamente) ate encontrar uma parede ou um caminho
 * já visitado:retorna falso e continua para os seguintes caminhos disponiveis
 * 2. avança até encontrar a saida: retorna verdadeiro e a função termina
 */
bool Labirinth::findGoal(int x, int y) {

	//caso base -> esta fora dos limites
	if (x >= 10 || y >= 10 || x < 0 || y < 0)
		return false;

	//caso base -> visitou este sitio
	if (visited[x][y]) {
		return false;
	}

	visited[x][y] = true;

	//caso base -> chegamos ao fim do labirinto
	if (labirinth[x][y] == 2)
		return true;

	//caso base -> encontrou uma parede
	if (labirinth[x][y] == 0)
		return false;

	//caso para a esquerda
	if (findGoal(x - 1, y))
		return true;

	//foi pela esquerda e não encontrou o final
	//caso para a direita
	if (findGoal(x + 1, y))
		return true;

	//foi pela direita e não encontrou o final
	//caso para cima
	if (findGoal(x, y - 1))
		return true;

	//foi para cima e não encontrou o final
	//caso para baixo
	if (findGoal(x, y + 1))
		return true;

	//se já esgotou todas as possibilidades de caminho, então voltar a biforcação anterior
	return false;
}

