/*
- The parameters of swap must be of the derived type, otherwise std::swap is called (a better match as it does not require a derived to base conversion)
*/

#pragma once

#include <vector>
#include <cassert>
#include "Element.h"

namespace BK_Sudoku
{
	class AbstractVector : public std::vector<Element>
	{
	public:
		typedef Element element_type;
		typedef element_type::bitfield bitfield;
		typedef unsigned char index_type;

		using vector::vector;
		AbstractVector() = default;
		AbstractVector(index_type puzzleOrder) : vector(puzzleOrder*puzzleOrder) {}
		//copy construction is a reference copy
		//copy assignment operation applies to individual elements of the vector
		AbstractVector& operator=(const AbstractVector &ev);

		void clear();
		bool empty();
		bool full();
		size_t countFilled();
		bitfield contains();
		bool contains(bitfield mask);
	private:
		static void copyVector(const AbstractVector &source, AbstractVector &dest);
	};

	class Block : public AbstractVector
	{
	public:
		//allocates Element data
		explicit Block(index_type puzzleOrder) : AbstractVector(puzzleOrder), order(puzzleOrder) {}
		using AbstractVector::operator=;
		
		element_type& accessElement(index_type row, index_type column);
		const element_type& accessElement(index_type row, index_type column) const;
	private:
		index_type order;
	};

	void swap(AbstractVector &ev1, AbstractVector &ev2);

	class RowCol : public AbstractVector
	{
	public:
		RowCol() = default;
		//range construction from another row or column only - no Element data allocation
		template<typename Iterator>
		RowCol(Iterator begin, Iterator end) : AbstractVector(begin, end) {}
		using AbstractVector::operator=;
	};

	void swap(RowCol &ev1, RowCol &ev2);
}