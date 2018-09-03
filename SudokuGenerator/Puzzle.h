#pragma once

#include "Grid.h"
#include "Solution.h"
#include <random>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <stack>

#define DISABLE_DEBUG

namespace BK_Sudoku
{
	class constraint_error : public std::runtime_error
	{
	public:
		using runtime_error::runtime_error;
	};

	class propagation_error : public std::runtime_error
	{
	public:
		using runtime_error::runtime_error;
	};

	class out_of_time : public std::runtime_error
	{
	public:
		using runtime_error::runtime_error;
	};

	template<typename Unit>
	class Stopwatch
	{
	public:
		typedef typename Unit::rep rep;

		explicit Stopwatch(rep timeLimit) : limit(timeLimit), active(false) {}
		
		void start() 
		{
			active = true;
			startTime = currentTime();
		}

		void stop()
		{
			active = false;
		}
		
		void reset(rep timeLimit = 0)
		{
			if (timeLimit != 0)
				limit = timeLimit;
			start();
		}
		
		void check()
		{
			assert(active);

			if ((currentTime() - startTime).count() > limit)
				throw out_of_time("Stopwatch time limit reached");
		}
	
	private:
		rep limit;
		Unit startTime;
		bool active;

		Unit currentTime()
		{
			return std::chrono::duration_cast<Unit>(std::chrono::system_clock::now().time_since_epoch());
		}
	};

	class Puzzle : public Grid
	{
	public:
		typedef Solution solution_type;
		typedef std::pair<int,int> pos_type;
		typedef element_type::bitfield bitfield;

		Puzzle(index_type puzzleOrder, size_t numberOfGivens);
		std::vector<Grid> enumerateSolutions(size_t max);
		void initialize();
	private:
		//seed solution
		solution_type solution;
		std::default_random_engine engine;
		size_t givens;
		Stopwatch<std::chrono::milliseconds> stopwatch;

		void initConstraints();
		std::pair<pos_type, pos_type> getPairOfRandomPositions();
		bool _generatePuzzle();
		//void refreshConstraints();
		//void initFullMask(bitfield &mask);
		void _enumerateSolutions(index_type row, index_type column, size_t howMany, std::vector<Grid> &solutions);
		void assignElement(index_type row, index_type column, bitfield value);
		void clearElement(index_type row, index_type column);
		//bool hasSolution();
		void propagateConstraints(index_type row, index_type column, std::stack<pos_type> &stk);

		void setConstraint(virtual_line_type &vr, bitfield mask);
		void _setConstraint(virtual_line_type::iterator it, virtual_line_type::iterator end_it, bitfield mask);
		void unsetConstraintRow(index_type row, bitfield mask);
		void unsetConstraintColumn(index_type column, bitfield mask);
		void unsetConstraintBlock(index_type row, index_type column, bitfield mask);
		void checkConstraints();
	};
}