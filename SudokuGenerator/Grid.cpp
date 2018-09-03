#include "Grid.h"
#include <stack>
#include <string>
#include <iomanip>

namespace BK_Sudoku
{
	std::ostream& operator<< (std::ostream& stream, const Grid &grid) 
	{
		for (int i = 0, j = grid.order * grid.order; i < j; ++i)
		{
			for (int k = 0; k < j; ++k)
			{
				auto &elem = grid.accessElement(i, k);
				stream << "(" << (elem.getNumericValue() > 0 ? std::to_string(elem.getNumericValue()) : " ") << ")";
		
			}
			stream << std::endl;
		}
		return stream;
	}

	Grid::Grid(unsigned char puzzleOrder)
		: order(puzzleOrder), grid(), file("debug.txt", std::ios_base::app)
	{
		initBlocks();
		//rows and columns are initialized with copies of the elements from the blocks
		initRows();
		initColumns();
	}

	Grid::Grid(const Grid &g)
		: order(g.order), grid()
	{
		initBlocks();
		initRows();
		initColumns();

		copyGrid(g, *this);
	}

	Grid&
		Grid::operator=(const Grid &g)
	{
		if (&g != this)
			copyGrid(g, *this);
		return *this;
	}

	void
		Grid::copyGrid(const Grid &source, Grid &dest)
	{
		assert(source.order == dest.order);

		auto dest_it = dest.grid.begin();
		for (auto source_it = source.grid.begin(), source_end = source.grid.end(); source_it != source_end; ++source_it, ++dest_it)
			*dest_it = *source_it;
	}

	Grid::element_type&
		Grid::accessElementAlt(unsigned char row, unsigned char column)
	{
		if (row >= order * order || column >= order * order)
			throw std::invalid_argument("Invalid row or column index");

		return locateBlock(row, column).accessElement(row % order, column % order);
	}

	Grid::element_type&
		Grid::accessElement(unsigned char row, unsigned char column)
	{
		return rows[row][column];
	}

	const Grid::element_type&
		Grid::accessElement(unsigned char row, unsigned char column) const
	{
		return rows[row][column];
	}

	Grid::block_type&
		Grid::locateBlock(unsigned char row, unsigned char column)
	{
		return grid[(row / order) * order + column / order];
	}

	const Grid::element_type&
		Grid::accessElementAlt(unsigned char row, unsigned char column) const
	{
		if (row >= order * order || column >= order * order)
			throw std::invalid_argument("Invalid row or column index");

		return locateBlock(row, column).accessElement(row % order, column % order);
	}

	const Grid::block_type&
		Grid::locateBlock(unsigned char row, unsigned char column) const
	{
		return grid[(row / order) * order + column / order];
	}

	Grid::block_type&
		Grid::accessBlock(unsigned char blockRow, unsigned char blockColumn)
	{
		if (blockRow >= order || blockColumn >= order)
			throw std::invalid_argument("Invalid block row or block column index");

		return grid[blockRow * order + blockColumn];
	}

	const Grid::block_type&
		Grid::accessBlock(unsigned char blockRow, unsigned char blockColumn) const
	{
		if (blockRow >= order || blockColumn >= order)
			throw std::invalid_argument("Invalid block row or block column index");

		return grid[blockRow * order + blockColumn];
	}

	void
		Grid::initRows()
	{
		for (int i = 0, j = order * order; i < j; ++i)
		{
			row_col_type row;
			for (int k = 0; k < j; ++k)
				row.emplace_back(accessElementAlt(i, k));
			rows.emplace_back(std::move(row));
		}
	}

	void
		Grid::initColumns()
	{
		for (int i = 0, j = order * order; i < j; ++i)
		{
			row_col_type column;
			for (int k = 0; k < j; ++k)
				column.emplace_back(accessElementAlt(k, i));
			columns.emplace_back(std::move(column));
		}
	}

	void
		Grid::initBlocks()
	{
		for (int i = 0, j = order*order; i < j; ++i)
			grid.emplace_back(Block{ order });
	}

	void
		Grid::clear()
	{
		for (auto it = grid.begin(), end_it = grid.end(); it != end_it; ++it)
			it->clear();
	}

	bool
		Grid::isEmpty()
	{
		for (auto it = grid.begin(), end_it = grid.end(); it != end_it; ++it)
			if (!it->empty())
				return false;
		return true;
	}

	size_t
		Grid::countFilled()
	{
		size_t ret = 0;
		for (int i = 0, j = order * order; i < j; ++i)
			for (int k = 0; k < j; ++k)
				if (!accessElement(i, k).isEmpty())
					++ret;
		return ret;
	}

	bool
		Grid::isFull()
	{
		for (auto it = grid.begin(), end_it = grid.end(); it != end_it; ++it)
			if (!it->full())
				return false;
		return true;
	}
}