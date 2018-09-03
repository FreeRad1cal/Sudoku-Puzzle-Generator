#include "Element.h"

namespace BK_Sudoku
{
	bool
		operator==(const Element &elem1, const Element &elem2)
	{
		return elem1.value == elem2.value;
	}

	void
		Element::deepCopy(const Element &source, Element &dest)
	{
		dest.orderedPair() = source.orderedPair();
		dest.getValue() = source.getValue();
		dest.elementConstraints() = source.elementConstraints();
	}

	void
		swap(Element &elem1, Element &elem2)
	{
		using std::swap;

		swap(elem1.orderedPair(), elem2.orderedPair());
		swap(elem1.getValue(), elem2.getValue());
		swap(elem1.elementConstraints(), elem2.elementConstraints());
	}

	size_t
		Element::howManyConstraints()
	{
		size_t count = 0;

		bitfield temp = elementConstraints();
		while (temp != 0)
		{
			count += (temp & 1);
			temp >>= 1;
		}

		return count;
	}
}