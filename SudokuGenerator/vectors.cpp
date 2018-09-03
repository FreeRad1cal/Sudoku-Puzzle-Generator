#include "vectors.h"

namespace BK_Sudoku
{
	AbstractVector& 
		AbstractVector::operator=(const AbstractVector &ev)
	{
		assert(size() == ev.size());

		if (&ev != this)
			copyVector(ev, *this);
		return *this;
	}

	void
		AbstractVector::copyVector(const AbstractVector &source, AbstractVector &dest)
	{
		auto source_it = source.begin();
		for (auto dest_it = dest.begin(), dest_end_it = dest.end(); dest_it != dest_end_it; ++dest_it, ++source_it)
			Element::deepCopy(*source_it, *dest_it);
	}

	void 
		swap(AbstractVector &ev1, AbstractVector &ev2)
	{
		if (&ev1 == &ev2)
			return;

		for (auto it1 = ev1.begin(), end_it1 = ev1.end(), it2 = ev2.begin(); it1 != end_it1; ++it1, ++it2)
		{
			swap(*it1, *it2);
		}
	}

	void
		AbstractVector::clear()
	{
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			it->clear();
	}

	bool
		AbstractVector::empty()
	{
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			if (!it->isEmpty())
				return false;
		return true;
	}

	size_t
		AbstractVector::countFilled()
	{
		size_t ret = 0;
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			if (!it->isEmpty())
				++ret;
		return ret;
	}

	bool
		AbstractVector::contains(element_type::bitfield mask)
	{
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			if (mask == it->getValue())
				return true;
		return false;
	}

	AbstractVector::bitfield
		AbstractVector::contains()
	{
		element_type::bitfield ret = 0;
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			ret |= it->getValue();
		return ret;
	}

	bool
		AbstractVector::full()
	{
		for (auto it = begin(), end_it = end(); it != end_it; ++it)
			if (it->isEmpty())
				return false;

		return true;
	}

//------------------------------------------------------------------------------

	Block::element_type& 
		Block::accessElement(index_type row, index_type column)
	{
		return (*this)[(order * row) + column];
	}

	const Block::element_type&
		Block::accessElement(index_type row, index_type column) const
	{
		return (*this)[(order * row) + column];
	}

	void
		swap(RowCol &ev1, RowCol &ev2)
	{
		swap(static_cast<AbstractVector>(ev1), static_cast<AbstractVector>(ev2));
	}
}