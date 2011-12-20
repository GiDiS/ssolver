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
#pragma once

#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>


typedef struct {
    char val;
    int excluded;
    char block;
    char blockOffset;
    char col;
    char row;
} SudokuSolutionCell;

typedef int SudokuSolutionStat[10];


typedef struct {
    SudokuSolutionCell cells[9][9];

    SudokuSolutionStat rStat;
    SudokuSolutionStat cStat;
    SudokuSolutionStat bStat;

    SudokuSolutionCell *rows[9][9];
    SudokuSolutionCell *cols[9][9];
    SudokuSolutionCell *blocks[9][9];

} SudokuSolution;

typedef char SudokuField[9][9];

class Sudoku
{

public:
    Sudoku();
    ~Sudoku();

    SudokuSolution* solve(SudokuField *field);
    void print(SudokuField *);
    void print(SudokuSolution *);
    
    bool verbose;
private:
    SudokuSolution *solution;

    int masks[10];
    int specialCases[512];

    void reset(void);
    void initIntVars(void);
    
    int setValue(char col, char row, char value);
    int setValue(SudokuSolutionCell * cell, char value);
    
    
    char solveNakedSingles(void);
    char solveHiddenSingles(void);
    char solveSwordfish(void);
};

#endif // SUDOKU_H
