/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Цисарук Владимир <vova@reklama-online.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Sudoku.h"
#include <string.h>

Sudoku::Sudoku()
{
    this->verbose = false;
    this->initIntVars();
}

void Sudoku::initIntVars()
{
    for (int i = 0; i < 512; i++) {
        this->specialCases[i] = 0;
    }
    for (int k, i = 1; i <= 9; i++) {
        k = 511 - (1<<(i-1));
        this->specialCases[k] = i;
//      std::cout << k << " " << i << "\n";
    }

    this->masks[0] = 0;
    for (int i = 1; i <= 9; i++) {
        this->masks[i] = 1 << (i - 1);
    }

    this->solution = new SudokuSolution;

    SudokuSolution *s = this->solution;
    SudokuSolutionCell *cell;

    char block, offset, col, row;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            cell = &s->cells[row][col];

            block = (row / 3) * 3 + col / 3;
            offset = (row % 3) * 3  + col % 3;

            s->blocks[block][offset] = cell;
            s->rows[row][col] = cell;
            s->cols[col][row] = cell;
  
            cell->block = block;
            cell->blockOffset = offset;
	    cell->row = row;
	    cell->col = col;

        }
    }

    this->reset();
}

void Sudoku::reset()
{
    SudokuSolution *s = this->solution;
    SudokuSolutionCell *cell;

    for (char i = 0; i < 9; i++) {
        s->rStat[i] = s->cStat[i] = s->bStat[i] = 0;
    }

    char block, offset, col, row;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            cell = &s->cells[row][col];

	    cell->val = 0;
            cell->excluded = 0;
        }
    }

}

Sudoku::~Sudoku()
{

}


SudokuSolution* Sudoku::solve(SudokuField *field)
{
    // Reset solution state
    this->reset();

    // Copy original values
    char col, row;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            this->setValue(row, col,(*field)[row][col]);
        }
    }

    char repeats, found = 0, wanted;

    repeats = 81 - found;
    do {
        found = this->solveNakedSingles();
        std::cout << "nakedSingles count: " << (int) found << "\n";
    } while (found > 0 && repeats-- > 0);

    repeats = 81 - found;
    do {
        found = this->solveHiddenSingles();
        std::cout << "hiddenSingles count: " << (int) found << "\n";
    } while (found > 0 && repeats-- > 0);


    return this->solution;
}

char Sudoku::solveNakedSingles(void )
{
    char found = 0;
    char col, row, value = 0;

    SudokuSolution *s = this->solution;
    SudokuSolutionCell *cell;

    for (row = 0; row < 9; row++) {

        if (s->rStat[row] == 0x1FF) continue;

        for (col = 0; col < 9; col++) {

            if (s->cStat[col] == 0x1FF) continue;

            cell = &s->cells[row][col];

            if (cell->val > 0) continue;

            value = this->specialCases[cell->excluded];
            if (value > 0) {
                this->setValue(cell, value);
                found++;
                continue;
            }
        }
    }

    return found;
}


char Sudoku::solveHiddenSingles(void )
{
    char singles = 0, val = 0, col, row;
    /*
    for (row = 0; row < 9; row++) {

      if (this->rowStat[row] == 0x1FF) continue;

      for (col = 0; col < 9; col++) {

     if (this->colStat[col] == 0x1FF) continue;
    if ((*this->solution)[row][col] > 0) continue;

    if ((val = this->specialCases[this->solutionMask[row][col]]) > 0) {
        this->checkSingle(col, row, val);
        singles++;
        continue;
    }
      }
    }
    */
    return singles;
}

char Sudoku::solveSwordfish(void )
{
    char singles = 0, val = 0, col, row , _col, _row;
    /*
    int skipCells[3][3], skipRows = 0, skipCols = 0;
    int mask = 0;
    for (val = 1; val < 9; val ++) {
      mask = this->masks[val];
      for (row = 0; row < 9; row++) {
    for (col = 0; col < 9; col++) {



    }
      }
    }
    */
    return 0;
}


int Sudoku::setValue(char row, char col, char value)
{
    return this->setValue(&this->solution->cells[row][col], value);
}

int Sudoku::setValue(SudokuSolutionCell *cell, char value)
{
    SudokuSolution *s = this->solution;
    
    if (this->verbose)
        std::cout  << "single " << (int) cell->row << " " << (int) cell->col << " " << (int) cell->val <<"\n";
    
    if (cell->val != value) {
        cell->val = value;
        cell->excluded = 0x1FF;
        int mask = this->masks[value];

        for (char i = 0; i < 9; i++) {
            s->cols[cell->col][i]->excluded |= mask;
            s->rows[cell->row][i]->excluded |= mask;
            s->blocks[cell->block][i]->excluded |= mask;
        }
        
        s->rStat[cell->row] |= mask;
        s->cStat[cell->col] |= mask;
        s->bStat[cell->block] |= mask;
    }

    return cell->excluded;
}


void Sudoku::print(SudokuField *field)
{
    std::cout << "Original\n";
    for (int col, row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            std::cout << (int) (*field)[row][col] << " ";

            if (col == 2 || col == 5)
                std::cout << "\t";
            if (col == 8) {
                std::cout << "\n";
                if (row == 2 || row == 5)
                    std::cout << "\n";
            }
        }
    }
}

void Sudoku::print(SudokuSolution *solution)
{
    SudokuSolutionCell *cell;
    std::cout << "Solution\n";
    for (int col, row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            cell = &solution->cells[row][col];
            std::cout << (int) cell->val << " ";

            if (col == 2 || col == 5)
                std::cout << "\t";
            if (col == 8) {
                std::cout << "\n";
                if (row == 2 || row == 5)
                    std::cout << "\n";
            }
        }
    }
}
