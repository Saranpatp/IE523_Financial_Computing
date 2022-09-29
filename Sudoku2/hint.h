/*  hint.h
        *  Sudoku
*  Created by Prof. Ramavarapu Sreenivas
*  Inspired by: http://web.eecs.utk.edu/courses/spring2012/cs140/Notes/Sudoku/index.html
*/

#ifndef sudoku
#define sudoku

#include <vector>
#include <fstream>
#include <tuple>
#include <set>
#include <map>

//using std::vector;
using namespace std;
class Sudoku
{
    // Private
    int puzzle[9][9];
    map<int,set<int>> posDiagMap; // nrow+ncol = const

    bool isBoard_valid(int row, int col, int value){
        //check row
        for (int j = 0; j < 9; j++) if (value == puzzle[row][j]) return false;

        //check col
        for (int i = 0; i < 9 ; i++) if (value == puzzle[i][col]) return false;

        int brow = row/3;
        int bcol = col/3;
        // check 3 x 3 block validity
        for (int i = 0 ; i < 3; i++){
            for (int j = 0 ; j < 3; j++)
                if (value == puzzle[brow*3 + i][bcol*3 + j]) return false;
        }
        return true;
    }

    bool positive_diagonals_valid(int row, int col, int value)
    {
        if (posDiagMap[row+col].find(value) != posDiagMap[row+col].end()) return false;
        return true;
    }

    bool adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4(int row, int col, int value)
    {
        if (row-1>=0 && col+1<9){
            if (abs(value - puzzle[row-1][col+1]) < 4) return false;
        }
        return true;
    }



public:
    void read_puzzle(int argc, char * const argv[]){
        int value;
        ifstream input_file(argv[1]);
//        ifstream input_file("input5.txt");
        if (input_file.is_open()){
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    input_file >> value;
                    puzzle[i][j] = value;
                    if (value != 0) {
                        if (posDiagMap[i+j].find(value) != posDiagMap[i+j].end()) {
                            cout << "The question is invalid diagonally "<<endl;
                            return;
                        }
                        posDiagMap[i+j].insert(value);
                    }
                }
            }
        }
    }

    // Public member function that prints the puzzle when called
    void print_puzzle()
    {
        //std::cout << number_of_calls << std::endl;
        std::cout << std::endl << "Board Position" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // check if we have a legitimate integer between 1 and 9
                if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    std::cout << puzzle[i][j] << " ";
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    std::cout << "X ";
                }
            }
            std::cout << std::endl;
        }
    }

    bool isPuzzleSloved(int& row, int& col){ // move col and row forward too
        for(row = 0; row < 9;row++)
            for (col = 0; col < 9; col++)
                if (puzzle[row][col] == 0) return false;
        print_puzzle();
        return true;
    }

    bool Solve(int row, int col)
    {
        if(isPuzzleSloved(row,col)) return true;
        for (int k = 1; k <= 9 ; k++){
            if (isBoard_valid(row,col,k) &&
                positive_diagonals_valid(row,col,k) &&
                adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4(row,col,k)
                    ) {
                puzzle[row][col] = k;
                posDiagMap[row+col].insert(k);
                Solve(row,col);
                posDiagMap[row+col].erase(k);
                puzzle[row][col]=0;
            }
        }
        return false;
    }
};

#endif