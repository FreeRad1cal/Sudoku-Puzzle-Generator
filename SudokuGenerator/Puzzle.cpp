#include "Puzzle.h"

namespace BK_Sudoku
{
	Puzzle::Puzzle(index_type puzzleOrder, size_t numberOfGivens)
		: Grid(puzzleOrder), solution(puzzleOrder), engine(Solution::generateSeed()), givens(numberOfGivens), stopwatch(200)
	{
		Grid::operator=(solution);
	}

	void
		Puzzle::initialize() 
	{
		while (true)
		{
			initConstraints();
			stopwatch.start();

			try
			{
				_generatePuzzle();
				//print puzzle
				//debug(1);
				break;
			}
			catch (out_of_time e)
			{
				solution.generateNewSolution();
				//initConstraints();
				Grid::operator=(solution);
				stopwatch.reset();
				continue;
			}
		}
	}

	bool
		Puzzle::_generatePuzzle()
	{
		stopwatch.check();

		if (enumerateSolutions(2).size() != 1) //if the solution is not unique
		{
			return false;
		}
		else if (countFilled() > givens) //if the desired number of givens has not yet been reached
		{
			while (true)
			{
				//get a random pair of symmetric positions
				auto posPair = getPairOfRandomPositions();

				element_type &elem1 = accessElement(posPair.first.first, posPair.first.second);
				element_type &elem2 = accessElement(posPair.second.first, posPair.second.second);

				//save the old values
				bitfield old1 = elem1.getValue(), old2 = elem2.getValue();

				//remove the elements (set values to 0)
				//file << "clearElement(" << posPair.first.first << ", " << posPair.first.second << ")" << std::endl;
				//file << "value: " << old1 << std::endl;
				//debug(1);
				clearElement(posPair.first.first, posPair.first.second);
				//debug(2);
				//checkConstraints();

				if (posPair.first != posPair.second)
				{
					//file << "clearElement(" << posPair.second.first << ", " << posPair.second.second << ")" << std::endl;
					//file << "value: " << old2 << std::endl;
					//debug(1);
					clearElement(posPair.second.first, posPair.second.second);
					//debug(2);
					//checkConstraints();
				}

				if (_generatePuzzle())
					return true;
				else
				{
					//put back the removed values and continue
					//file << "assignElement(" << posPair.first.first << ", " << posPair.first.second << ")" << std::endl;
					//file << "value: " << old1 << std::endl;
					//debug(1);
					assignElement(posPair.first.first, posPair.first.second, old1);
					//debug(2);
					//checkConstraints();
					if (posPair.first != posPair.second)
					{
						//file << "assignElement(" << posPair.second.first << ", " << posPair.second.second << ")" << std::endl;
						//file << "value: " << old2 << std::endl;
						//debug(1);
						assignElement(posPair.second.first, posPair.second.second, old2);
						//debug(2);
						//checkConstraints();
					}
				}

			}
		}
		else //	countSolutions() == 1 && cellsFilled() == givens
			return true;
	}

	std::pair<Puzzle::pos_type, Puzzle::pos_type>
		Puzzle::getPairOfRandomPositions()
	{
		static std::vector<pos_type> positions;
		//a no-op if the positions vector is not empty
		static auto initPositions = 
		[this](std::vector<pos_type> &positions) ->void
		{
			if (!positions.empty())
				return;

			for (int i = 0, j = order*order; i < j; ++i)
				for (int k = 0; k < j; ++k)
					positions.push_back(pos_type(i, k));
		};
		initPositions(positions);

		pos_type pos, compl_pos;

		do
		{
			pos = positions[engine() % positions.size()];
		} while (accessElement(pos.first, pos.second).isEmpty());

		compl_pos.first = order * order - 1 - pos.first;
		compl_pos.second = order * order - 1 - pos.second;

		return std::make_pair(pos, compl_pos);
	}

	//void
	//	Puzzle::initFullMask(bitfield &mask)
	//{
	//	if (mask != 0)
	//		return;

	//	bitfield temp = 1;
	//	for (int i = 0, j = order*order; i < j; ++i, temp <<= 1)
	//		mask |= temp;
	//}

	void Puzzle::initConstraints()
	{
		for (int i = 0, j = order*order; i < j; ++i)
			for (int k = 0; k < j; ++k)
				accessElement(i, k).elementConstraints() = 0;
	}

	//void
	//	Puzzle::refreshConstraints()
	//{
	//	static bitfield mask = 0;
	//	//a no-op if mask != 0
	//	initFullMask(mask);

	//	for (int i = 0, j = order*order; i < j; ++i)
	//	{
	//		bitfield rowMask = accessRow(i).contains();

	//		for (int k = 0; k < j; ++k)
	//		{
	//			bitfield colMask = accessColumn(k).contains();
	//			bitfield blockMask = locateBlock(i, k).contains();
	//			bitfield eliminatedChoices = rowMask | colMask | blockMask;
	//			accessElement(i, k).elementConstraints() = mask ^ eliminatedChoices;
	//		}
	//	}
	//}

	std::vector<Grid>
		Puzzle::enumerateSolutions(size_t howMany)
	{
		std::vector<Grid> solutions;
		_enumerateSolutions(0, 0, howMany, solutions);
		//if (!solutions.empty())
		//{
		//	std::cout << solutions.size() << std::endl;
		//	for (auto it = solutions.begin(), end_it = solutions.end(); it != end_it; ++it)
		//		it->debug(1);
		//}

		return std::move(solutions);
	}

	void
		Puzzle::_enumerateSolutions(index_type row, index_type column, size_t max, std::vector<Grid> &solutions)
	{
		if (row == order*order || solutions.size() == max) //the end of the grid or the required number of solutions has been reached
			return;
		else if (isFull()) //a solution has been generated
			solutions.emplace_back(*this);
		else
		{
			element_type &elem = accessElement(row, column);
			index_type nextRow = row + ((column + 1) / (order*order));
			index_type nextColumn = (column + 1) % (order*order);

			if (elem.isEmpty())
			{
				//iterate over the possible values
				for (bitfield field = 1; field <= elem.elementConstraints(); field <<= 1)
				{
					//if the value is not in the constraint bitfield
					if ((field & elem.elementConstraints()) == 0)
						continue;

					std::stack<pos_type> stk;
					try
					{
						assignElement(row, column, field); //throws constraint_error
						propagateConstraints(0, 0, stk); //throws propagation_error
						_enumerateSolutions(nextRow, nextColumn, max, solutions);
					}
					catch (constraint_error e)
					{
						return;
					}
					catch (propagation_error e)
					{
						clearElement(row, column);
						return;
					}

					//undo the constraint propagation
					while (!stk.empty())
					{
						clearElement(stk.top().first, stk.top().second);
						stk.pop();
					}
					//undo the assignment
					clearElement(row, column);
				}
			}
			else
				_enumerateSolutions(nextRow, nextColumn, max, solutions);

		}
	}

	void
		Puzzle::assignElement(index_type row, index_type column, bitfield value)
	{
		element_type &elem = accessElement(row, column);
		assert(elem.isEmpty());

		elem.getValue() = value;
		
		try
		{
			setConstraint(locateBlock(row, column), value);
			setConstraint(accessRow(row), value);
			setConstraint(accessColumn(column), value);
		}
		catch (constraint_error &e)
		{
			clearElement(row, column);
			throw e;
		}

		//try
		//{ setConstraint(locateBlock(row, column), value); }
		//catch (constraint_error &e)
		//{ elem.clear(); throw e; }

		//try
		//{ setConstraint(accessRow(row), value); }
		//catch (constraint_error &e)
		//{ elem.clear(); throw e; }

		//try
		//{ setConstraint(accessColumn(column), value); }
		//catch (constraint_error &e)
		//{ elem.clear(); throw e; }
	}

	void
		Puzzle::clearElement(index_type row, index_type column)
	{
		element_type &elem = accessElement(row, column);
		bitfield current = elem.getValue();

		elem.clear();
		//elem.elementConstraints() &= ~current;

		unsetConstraintRow(row, current);
		unsetConstraintColumn(column, current);
		unsetConstraintBlock(row, column, current);
	}

	//bool Puzzle::hasSolution()
	//{
	//	for (int i = 0, j = order*order; i < j; ++i)
	//		for (int k = 0; k < j; ++k)
	//		{
	//			element_type &elem = accessElement(i, k);
	//			if (elem.empty() && elem.howManyConstraints() == 0)
	//				return false;
	//		}
	//	return true;
	//}

	void
		Puzzle::propagateConstraints(index_type row, index_type column, std::stack<pos_type> &stk)
	{
		if (row == order*order)
			return;

		index_type nextRow = row + ((column + 1) / (order*order));
		index_type nextColumn = (column + 1) % (order*order);
		element_type &elem = accessElement(row, column);

		if (elem.isEmpty() && elem.howManyConstraints() == 1)
		{
			stk.push(pos_type(row, column));
			try
			{
				assignElement(row, column, elem.elementConstraints()); //throws constraint_error
				propagateConstraints(nextRow, nextColumn, stk); //throws propagation_error
			}
			catch (constraint_error e)
			{
				stk.pop();
				throw propagation_error("Conflict detected during constraint propagation");
			}
			catch (propagation_error &e)
			{
				clearElement(row, column);
				stk.pop();
				throw e;
			}
		}
		else
			propagateConstraints(nextRow, nextColumn, stk);

	}

	void
		Puzzle::setConstraint(virtual_line_type &vr, bitfield mask)
	{
		//setting a constraint unsets the bitflag specified by mask
		_setConstraint(vr.begin(), vr.end(), mask);
	}

	void
		Puzzle::unsetConstraintBlock(index_type row, index_type column, bitfield mask)
	{
		auto &block = locateBlock(row, column);

		//unsetting a constraint sets the bitflag specified by mask
		index_type rowOffset = order * (row / order);
		index_type columnOffset = order * (column / order);
		for (int i = 0; i < order; ++i)
			for (int j = 0; j < order; ++j)
				if (!accessRow(i + rowOffset).contains(mask) && !accessColumn(j + columnOffset).contains(mask))
					block.accessElement(i,j).elementConstraints() |= mask;

		////unsetting a constraint sets the bitflag specified by mask
		//for (auto it = block.begin(), end_it = block.end(); it != end_it; ++it)
		//	//if (it->empty())
		//	it->elementConstraints() |= mask;
	}

	void
		Puzzle::unsetConstraintRow(index_type row, bitfield mask)
	{
		//unsetting a constraint sets the bitflag specified by mask
		auto &r = accessRow(row);
		index_type columnIndex = 0;
		for (auto it = r.begin(), end_it = r.end(); it != end_it; ++it, ++columnIndex)
		{
			if (!accessColumn(columnIndex).contains(mask) && !locateBlock(row, columnIndex).contains(mask))
				it->elementConstraints() |= mask;
		}
	}

	void
		Puzzle::unsetConstraintColumn(index_type column, bitfield mask)
	{
		//unsetting a constraint sets the bitflag specified by mask
		auto &c = accessColumn(column);
		index_type rowIndex = 0;
		for (auto it = c.begin(), end_it = c.end(); it != end_it; ++it, ++rowIndex)
		{
			if (!accessRow(rowIndex).contains(mask) && !locateBlock(rowIndex, column).contains(mask))
				it->elementConstraints() |= mask;
		}
	}

	void
		Puzzle::_setConstraint(virtual_line_type::iterator it, virtual_line_type::iterator end_it, bitfield mask)
	{
		if (it == end_it)
			return;
		else
		{
			bitfield oldConstraints = it->elementConstraints();
			it->elementConstraints() &= ~mask;
			if ((it->isEmpty() && it->elementConstraints() == 0))
			{
				it->elementConstraints() = oldConstraints;
				throw constraint_error("Failure setting constraint for a block");
			}

			try
			{
				_setConstraint(it + 1, end_it, mask);
			}
			catch (...)
			{
				it->elementConstraints() = oldConstraints;
				throw;
			}
		}
	}

	void Puzzle::checkConstraints()
	{
		for (int i = 0; i < order; ++i)
			for (int j = 0; j < order; ++j)
			{
				element_type &elem = accessElement(i, j);
				auto &block = locateBlock(i, j);
				auto &row = accessRow(i);
				auto &column = accessColumn(j);

				bitfield mask = 1;
				bitfield value = elem.getValue();
				bitfield constraints = elem.elementConstraints();
				while (mask <= constraints)
				{
					if ((mask & constraints) != 0)
					{
						assert(!block.contains(mask));
						assert(!row.contains(mask));
						assert(!column.contains(mask));
					}
					else
					{
						assert(block.contains(mask) || row.contains(mask) || column.contains(mask));
					}

					mask <<= 1;
				}
			}
	}
}