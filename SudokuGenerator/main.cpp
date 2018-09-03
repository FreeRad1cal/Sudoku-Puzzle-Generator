#include "Puzzle.h"

int main() {
	auto puzzle = BK_Sudoku::Puzzle(3, 23);
	puzzle.initialize();
	std::cout << puzzle;
	char x;
	std::cin >> x;
}