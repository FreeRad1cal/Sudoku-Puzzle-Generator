# Sudoku-Puzzle-Generator
This library generates Sudoku puzzles of the specified size (order) and with the specified number of given digits.

## Using the library

Include the Puzzle.h header file:
```c++
#include "Puzzle.h"
```

The puzzle is generated as follows:
```c++
// Generate a 9x9 Sudoku puzzle with 37 given digits
BK_Sudoku::Puzzle puzzle(3, 37);
puzzle.initialize();
std::cout << puzzle;
```
The constructor of the `Puzzle` class takes two arguments: the order of the puzzle and the number of given digits. The order is the size of the inner regions. That is, a puzzle with the order of `3` has 3x3 inner regions and therefore the total grid size of 9x9. The library generates puzzles that have a unique solution. For 9x9 puzzles, the algorithm remains sufficiently performant down to about 23 given digits. 

The sample output is:
```
( )(2)(3)(1)( )( )( )( )(5)
( )( )(6)( )(8)(5)(4)( )(3)
( )(7)(8)( )( )(2)( )( )( )
(3)(4)( )( )(1)( )( )( )(8)
( )( )( )( )( )( )( )( )( )
(8)( )( )(2)(3)( )( )(6)(1)
( )( )( )( )( )( )(8)(5)( )
( )( )(1)(8)(5)( )(2)( )( )
(7)( )( )( )(2)(3)(1)( )( )
```

The `Puzzle` class exposes a set of iterators for the rows, columns and the inner grids (blocks), as well as methods that allow random access to the same:
```c++
//random access to blocks
block_type& accessBlock(unsigned char blockRow, unsigned char blockColumn);
const block_type& accessBlock(unsigned char blockRow, unsigned char blockColumn) const;

//sequential iteration over blocks
block_iterator blockBegin();
const_block_iterator blockBegin() const;
block_iterator blockEnd();
const_block_iterator blockEnd();

//sequential iteration over rows
row_col_iterator rowsBegin();
const_row_col_iterator rowsBegin() const;
row_col_iterator rowsEnd();
const_row_col_iterator rowsEnd() const;

//random access to rows
row_col_type& accessRow(unsigned char row);
const row_col_type& accessRow(unsigned char row) const;

//sequential iteration over columns
row_col_iterator columnsBegin();
const_row_col_iterator columnsBegin() const;
row_col_iterator columnsEnd();
const_row_col_iterator columnsEnd() const;

//random access to columns
row_col_type& accessColumn(unsigned char column);
const row_col_type& accessColumn(unsigned char column) const;

// Print the first element of each row to std::cout
for (auto row = puzzle.rowsBegin(); row < puzzle.rowsEnd(); ++row)
    std::cout << (*row)[0].getNumericValue() << std::endl;
```
