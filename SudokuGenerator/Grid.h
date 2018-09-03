#pragma once

#include <utility>
#include <random>
#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include <iterator>
#include <cassert>
#include "element.h"
#include "vectors.h"
#include <fstream>
#include <iostream>

namespace BK_Sudoku
{
	class Grid
	{
	public:
		typedef AbstractVector virtual_line_type;
		typedef Block block_type;
		typedef RowCol row_col_type;

		typedef Block::element_type element_type;
		typedef Block::element_type::bitfield bitfield;
		typedef Block::index_type index_type;

		typedef std::vector<block_type>::iterator block_iterator;
		typedef std::vector<block_type>::const_iterator const_block_iterator;

		typedef std::vector<block_type>::iterator block_iterator;
		typedef std::vector<block_type>::const_iterator const_block_iterator;
		typedef std::vector<row_col_type>::iterator row_col_iterator;
		typedef std::vector<row_col_type>::const_iterator const_row_col_iterator;

		explicit Grid(unsigned char puzzleOrder);
		Grid(const Grid&);
		
		virtual ~Grid() = default;
		
		Grid& operator=(const Grid &g);
		
		//random access to elements
		element_type& accessElement(unsigned char row, unsigned char column);
		const element_type& accessElement(unsigned char row, unsigned char column) const;

		//random access to elements
		element_type& accessElementAlt(unsigned char row, unsigned char column);
		const element_type& accessElementAlt(unsigned char row, unsigned char column) const;
		
		//finding the location of a block from the location of an element
		block_type& locateBlock(unsigned char row, unsigned char column);
		const block_type& locateBlock(unsigned char row, unsigned char column) const;

		//random access to blocks
		block_type& accessBlock(unsigned char blockRow, unsigned char blockColumn);
		const block_type& accessBlock(unsigned char blockRow, unsigned char blockColumn) const;

		//sequential iteration over blocks
		block_iterator blockBegin() { return grid.begin(); }
		const_block_iterator blockBegin() const { return grid.begin(); }
		block_iterator blockEnd() { return grid.end(); }
		const_block_iterator blockEnd() const { return grid.end(); }

		//sequential iteration over rows
		row_col_iterator rowsBegin() { return rows.begin(); }
		const_row_col_iterator rowsBegin() const { return rows.begin(); }
		row_col_iterator rowsEnd() { return rows.end(); }
		const_row_col_iterator rowsEnd() const { return rows.end(); }

		//random access to rows
		row_col_type& accessRow(unsigned char row) { return rows[row]; }
		const row_col_type& accessRow(unsigned char row) const { return rows[row]; }

		//sequential iteration over columns
		row_col_iterator columnsBegin() { return columns.begin(); }
		const_row_col_iterator columnsBegin() const { return columns.begin(); }
		row_col_iterator columnsEnd() { return columns.end(); }
		const_row_col_iterator columnsEnd() const { return columns.end(); }

		//random access to columns
		row_col_type& accessColumn(unsigned char column) { return columns[column]; }
		const row_col_type& accessColumn(unsigned char column) const { return columns[column]; }

		void clear();
		bool isEmpty();
		size_t countFilled();
		bool isFull();
		
		friend std::ostream& operator<< (std::ostream& stream, const Grid &grid);
		
	protected:
		unsigned char order;
		//master grid
		std::vector<block_type> grid;
		//copies of the elements from the master grid - all data is shared
		std::vector<row_col_type> rows;
		std::vector<row_col_type> columns;
		std::ofstream file;

		void initBlocks();
		void initRows();
		void initColumns();
		void copyGrid(const Grid &source, Grid &dest);
	};
}