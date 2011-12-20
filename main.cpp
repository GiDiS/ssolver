#include <iostream>
#include <time.h>
#include "sudoku/Sudoku.h"


// http://sudoku.leit.ru/sudoku.php?l=5

int main(int argc, char **argv) {
    
    clock_t t_start = clock();
    
    SudokuSolution *solution;
    
    
    SudokuField field1 = {
      {8,7,0, 0,0,1, 4,0,0},
      {3,0,0, 2,0,4, 0,0,0},
      {0,0,0, 0,0,0, 0,0,9},
      {0,0,1, 0,0,6, 0,9,4},
      {9,0,0, 3,0,7, 0,0,8},
      {2,8,0, 4,0,0, 6,0,0},
      {5,0,0, 0,0,0, 0,0,0},
      {0,0,0, 1,0,9, 0,0,3},
      {0,0,4, 5,0,0, 0,2,7},
    };

    
    
    SudokuField field2 = {
      {0,1,0, 0,0,0, 8,0,3},
      {5,0,0, 0,9,6, 1,0,0},
      {0,0,4, 0,8,1, 0,6,0},
    
      {9,0,0, 4,0,3, 0,0,6},
      {0,2,0, 0,6,0, 0,1,0},
      {0,0,0, 8,0,5, 0,0,7},
      
      {0,6,0, 0,3,0, 4,0,1},
      {0,0,0, 1,7,0, 6,3,5},
      {1,0,3, 6,0,0, 0,2,0},
    };
    
    Sudoku *sudoku =new Sudoku();
    
    sudoku->print(&field1);
    solution = sudoku->solve(&field1);
    sudoku->print(solution);

    std::cout << std::endl << std::endl;

    sudoku->print(&field2);
    solution = sudoku->solve(&field2);
    sudoku->print(solution);

    clock_t t_end = clock();
    
    int diff = (t_end - t_start) / (CLOCKS_PER_SEC / 1000);
    std::cout <<  "Time elapsed: " << diff;
    return 0;
}
