#pragma once

#include "Grid.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

namespace BK_Sudoku
{
	class Solution : public Grid
	{
	public:
		typedef element_type::pair_type pair_type;
		typedef element_type::bitfield bitfield;

		using Grid::Grid;
		Solution(unsigned char puzzleOrder);
		using Grid::operator=;

		void generateNewSolution();
		static size_t generateSeed();

	private:
		std::default_random_engine engine;

		void fillBinaryPairs();
		void fillFirstComponent();
		void fillSecondComponent();
		void permuteRows();
		void permuteColumns();
		void fillValues();
	};
}