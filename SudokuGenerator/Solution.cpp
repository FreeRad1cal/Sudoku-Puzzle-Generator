#include "Solution.h"

namespace BK_Sudoku
{
	Solution::Solution(unsigned char puzzleOrder)
		: Grid(puzzleOrder), engine(generateSeed())
	{
		generateNewSolution();
	}

	void
		Solution::generateNewSolution()
	{
		static std::bernoulli_distribution dist;

		fillBinaryPairs();

		if (dist(engine))
			permuteRows();
		else
			permuteColumns();

		fillValues();
	}

	void Solution::fillBinaryPairs()
	{
		fillFirstComponent();
		fillSecondComponent();
	}

	void
		Solution::fillFirstComponent()
	{
		typedef Grid::element_type::pair_type pair_type;

		for (int i = 0; i < order; ++i)
		{
			for (int j = 0; j < order; ++j)
			{
				auto &block = accessBlock(i, j);
				int firstCompVal = (i + j) % order;
				for (auto it = block.begin(), end_it = block.end(); it != end_it; ++it)
					it->orderedPair().first = firstCompVal;
			}
		}
	}

	void
		Solution::fillSecondComponent()
	{
		typedef Grid::element_type::pair_type pair_type;

		for (auto block = blockBegin(), block_end = blockEnd(); block != block_end; ++block)
			for (int i = 0; i < order; ++i)
				for (int j = 0; j < order; ++j)
					block->accessElement(i, j).orderedPair().second = (i + j) % order;
	}

	size_t
		Solution::generateSeed()
	{
		return std::chrono::system_clock::now().time_since_epoch().count() % 10000;
	}

	void
		Solution::permuteRows()
	{
		//a temporary grid
		Grid newGrid(order);

		//a vector to hold block row indexes in a random permutation (e.g., if order == 3, then blockIndexes holds 0,1,2)
		std::vector<int> blockIndexes;
		for (int i = 0; i < order; ++i)
			blockIndexes.push_back(i);

		//copy rows from internal grid to newGrid, so that no block contains two rows with the same first binary component
		for (int i = 0; i < order; ++i)
		{
			std::shuffle(blockIndexes.begin(), blockIndexes.end(), engine);
			int j = 0;
			for (auto random_it = blockIndexes.begin(), random_end_it = blockIndexes.end(); random_it != random_end_it; ++random_it, ++j)
				newGrid.rowsBegin()[(*random_it) * order + i] = rowsBegin()[i * order + j];
		}

		//shuffle the rows within each row of blocks --- BUGGY
		for (auto it = newGrid.rowsBegin(), end_it = it, tot_end = newGrid.rowsEnd(); it != tot_end; it += order)
		{
			end_it += order;
			std::shuffle(it, end_it, engine);
		}

		Grid::operator=(newGrid);
	}

	void
		Solution::permuteColumns()
	{
		//a temporary grid
		Grid newGrid(order);

		//a vector to hold block column indexes in a random permutation (e.g., if order == 3, then blockIndexes holds 0,1,2)
		std::vector<int> blockIndexes;
		for (int i = 0; i < order; ++i)
			blockIndexes.push_back(i);

		//copy columns from internal grid to newGrid, so that no block contains two columns with the same first binary component
		for (int i = 0; i < order; ++i)
		{
			std::shuffle(blockIndexes.begin(), blockIndexes.end(), engine);
			int j = 0;
			for (auto random_it = blockIndexes.begin(), random_end_it = blockIndexes.end(); random_it != random_end_it; ++random_it, ++j)
				newGrid.columnsBegin()[(*random_it) * order + i] = columnsBegin()[i * order + j];
		}

		//shuffle the columns within each row of blocks
		for (auto it = newGrid.columnsBegin(), end_it = it, tot_end = newGrid.columnsEnd(); it != tot_end; it += order)
		{
			end_it += order;
			std::shuffle(it, end_it, engine);
		}

		Grid::operator=(newGrid);
	}

	void
		Solution::fillValues()
	{
		//a vector to hold randomly generated bitfields
		std::vector<bitfield> bitfields;
		bitfield field = 1;
		for (int i = 0, j = order*order; i < j; ++i, field <<= 1)
			bitfields.emplace_back(field);

		//shuffle the bitfield vector
		std::shuffle(bitfields.begin(), bitfields.end(), engine);

		//generate the hash map
		std::vector<std::vector<bitfield>> hash_map(order, std::vector<bitfield>(order));
		auto bitfields_it = bitfields.begin();
		for (int i = 0, j = order; i < j; ++i)
			for (int k = 0; k < j; ++k, ++bitfields_it)
				hash_map[i][k] = *bitfields_it;

		//fill the values
		for (int i = 0, j = order*order; i < j; ++i)
			for (int k = 0; k < j; ++k)
			{
				auto &elem = accessElement(i, k);
				elem.getValue() = hash_map[elem.orderedPair().first][elem.orderedPair().second];
			}
	}
}