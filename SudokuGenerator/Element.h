#pragma once
#include <utility>
#include <memory>

namespace BK_Sudoku
{
	//Pointer-like element class
	class Element
	{
		friend void swap(Element&, Element&);
		friend bool operator==(const Element&, const Element&);
	public:
		typedef size_t bitfield;
		typedef std::pair<unsigned char, unsigned char> pair_type;
		typedef std::shared_ptr<bitfield> bitfield_ptr;
		typedef std::shared_ptr<pair_type> pair_type_ptr;

		Element() : pair(std::make_shared<pair_type>(0, 0)), value(std::make_shared<bitfield>(0)), constraints(std::make_shared<bitfield>(0)) {}
		explicit Element(const pair_type &pair) : pair(std::make_shared<pair_type>(0, 0)), value(std::make_shared<bitfield>(0)), constraints(std::make_shared<bitfield>(0)) {}

		pair_type& orderedPair() { return *pair; }
		const pair_type& orderedPair() const { return *pair; }

		bitfield& getValue() { return *value; }
		const bitfield& getValue() const { return *value; }

		const unsigned int getNumericValue() const { return (unsigned int)(log(getValue()) / log(2)); }

		bitfield& elementConstraints() { return *constraints; }
		const bitfield& elementConstraints() const { return *constraints; }

		static void deepCopy(const Element &source, Element &dest);
		void clear() { getValue() = 0; }
		bool isEmpty() { return getValue() == 0; }
		size_t howManyConstraints();
	private:
		pair_type_ptr pair;
		bitfield_ptr value;
		bitfield_ptr constraints;
	};
}