/*
 * Sudoku.cpp
 *
 */

#include "Sudoku.h"
#include <tuple>
#include <vector>

/** Inicia um Sudoku vazio.
 */
Sudoku::Sudoku() {
	this->initialize();
}

/**
 * Inicia um Sudoku com um conteúdo inicial.
 * Lança excepção IllegalArgumentException se os valores
 * estiverem fora da gama de 1 a 9 ou se existirem números repetidos
 * por linha, coluna ou bloc 3x3.
 *
 * @param nums matriz com os valores iniciais (0 significa por preencher)
 */
Sudoku::Sudoku(int nums[9][9]) {
	this->initialize();

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (nums[i][j] != 0) {
				int n = nums[i][j];
				numbers[i][j] = n;
				lineHasNumber[i][n] = true;
				columnHasNumber[j][n] = true;
				block3x3HasNumber[i / 3][j / 3][n] = true;
				countFilled++;
			}
		}
	}
}

void Sudoku::initialize() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int n = 0; n < 10; n++) {
				numbers[i][j] = 0;
				lineHasNumber[i][n] = false;
				columnHasNumber[j][n] = false;
				block3x3HasNumber[i / 3][j / 3][n] = false;
			}
		}
	}

	this->countFilled = 0;
}

/**
 * Obtem o conteúdo actual (só para leitura!).
 */
int** Sudoku::getNumbers() {
	int** ret = new int*[9];

	for (int i = 0; i < 9; i++) {
		ret[i] = new int[9];

		for (int a = 0; a < 9; a++)
			ret[i][a] = numbers[i][a];
	}

	return ret;
}

/**
 * Verifica se o Sudoku já está completamente resolvido
 */
bool Sudoku::isComplete() {
	return countFilled == 9 * 9;
}

/*
 *@brief Chooses the nextCel with a greedy algorithm:
 *@brief the cell with the fewest options
 *@brief in case the case of multiple options, returns the first one found
 *
 *@return returns the cell with the fewest options
 */
tuple<int, int, vector<int>> Sudoku::nextCel() {

	//let's save the best cell (the cell with the fewest options)
	int best_x = 0;
	int best_y = 0;
	vector<int> vec;

	//saves the number of options for the cell above.
	int best_n = 9;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (numbers[i][j] == 0) {
				int n = 9;
				vector<int> vec_tmp;

				//checks what numbers are in the column, line or block
				for (int a = 1; a <= 9; a++)
					if (columnHasNumber[j][a] || lineHasNumber[i][a]
							|| block3x3HasNumber[i / 3][j / 3][a])
						{
							n--;
							vec_tmp.push_back(a);
						}

				//saves the cell with fewest options
				if (n < best_n) {
					best_n = n;
					best_x = i;
					best_y = j;
				}

				if (n == 1)
					break;
			}
		}

	return make_tuple(best_x, best_y, vec);
}

/**
 *@brief Fills the cell with the coordenates x and y
 *@brief with a value and updates everything
 *
 */
void Sudoku::fillCel(int x, int y, int value) {
	numbers[x][y] = value;
	lineHasNumber[x][value] = true;
	columnHasNumber[y][value] = true;
	block3x3HasNumber[x / 3][y / 3][value] = true;
	countFilled++;
}

/**
 *@brief Empties the cell with the coordenates x and y
 *@brief and updates everything
 *
 */
void Sudoku::emptyCel(int x, int y, int value) {
	numbers[x][y] = 0;
	lineHasNumber[x][value] = false;
	columnHasNumber[y][value] = false;
	block3x3HasNumber[x / 3][y / 3][value] = false;
	countFilled--;
}

/**
 *@brief Solves the soduku (recursively)
 *@brief Auxiliary function do Solve
 */
bool Sudoku::solveRec(int x, int y, vector<int> vec) {
	//Caso base
	if (isComplete())
		return true;

	//Iterar de 1 a 9 (para os valores da celula)
	for (unsigned int i = 0; i < vec.size(); i++) {

			fillCel(x, y, i);

		tuple<int, int, vector<int>> cel = nextCel();

		if (solveRec(get<0>(cel), get<1>(cel), get<2>(cel)))
			return true;
		else
			emptyCel(x, y, i);
	}

	return false;
}

/**
 * Resolve o Sudoku.
 * Retorna indicação de sucesso ou insucesso (sudoku impossível).
 */
bool Sudoku::solve() {

	tuple<int, int, vector<int>> cel = nextCel();

	return solveRec(get<0>(cel), get<1>(cel), get<2>(cel));
}

/**
 * Imprime o Sudoku.
 */
void Sudoku::print() {
	for (int i = 0; i < 9; i++) {
		for (int a = 0; a < 9; a++)
			cout << this->numbers[i][a] << " ";

		cout << endl;
	}
}
