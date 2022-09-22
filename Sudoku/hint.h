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

    bool isBoard_valid(int row, int col){
        set<int> rows;
        set<int> cols;
        set<int> board;
        //check row
        for (int j = 0; j < 9; j++){
            int value = puzzle[row][j];
            if (value==0) continue;
            if (rows.find(value) != rows.end()) return false;
            rows.insert(value);
        }
        //check col
        for (int i = 0; i < 9 ; i++){
            int value = puzzle[i][col];
            if (value==0) continue;
            if (cols.find(value) != cols.end()) return false;
            cols.insert(value);
        }

        int brow = row/3;
        int bcol = col/3;
        // check 3 x 3 block validity
        for (int i = 0 ; i < 3; i++){
            for (int j = 0 ; j < 3; j++){
                int value = puzzle[brow*3 + i][bcol*3 + j];
                if (value == 0) continue;
                if ( board.find(value) != board.end()) return false;
                board.insert(value);
            }
        }
        return true;
    }


    // Private member function that checks if the positive-diagonal
    // (SW to NE diagonals) have no repeated digits. If the initial
    // partially-filled puzzle does not meet this requirement, I need
    // to flag this (see write-up). I am using tuples for this. This
    // way I can show which value is repeated along the diagonal multiple times
    // I took this from https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
    std::tuple<bool, int> positive_diagonals_valid()
    {
        // write code that checks if there are any repeated
        // digits along the 15-many positive diagonals
        // If the intial puzzle does not meet this requirement,
        // I want to let the user know that digit X has appeared
        // multiple times along some positive diagonal (in the
        // partially-filled input puzzle). This way the user can
        // see the reason why there can be no solution to the
        // Dutch Miracle Sudoku puzzle instance.
        //
        // In my implementation, if the returned bool variable is false,
        // then the second-tuple is the X that was repeated along
        // some positive diagonal (see description of assignment
        // for a sample output).
        // add set or map here to check that it is valid
        //check first half
        map<int,set<int>> posDiagMap; // nrow+ncol = const
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9;j++){
                int value = puzzle[i][j];
                if (value == 0) continue;
                if (posDiagMap[i+j].find(value) != posDiagMap[i+j].end()) return {false,value};
                posDiagMap[i+j].insert(value);
            }
        }
        return {true,0};

    }

    // Private member funtion that checks if adjacent cells
    // (along the positive diagonal) touching at a corner
    // have a difference of atleast 4
    std::tuple<bool, int, int> adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4()
    {
        // write code that checks if all non-zero entries in the
        // puzzle (filled so far) have met the "minimum-difference
        // of 4" rule.
        //
        // In my implementation, if the returned bool variable is false,
        // then the two ints (i and j, say) identify puzzle[i][j] and
        // puzzle[i-1][j+1] that do not meet the "minimum-difference
        // of 4" (see program assignment description for a sample output)
        //check first half
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9;j++){
                int value = puzzle[i][j];
                if (value == 0) continue;
                if (i-1>=0 && j+1<9){
                    if (puzzle[i-1][j+1] == 0) continue;
                    if (abs(value - puzzle[i-1][j+1]) <4) return {false,i,j};
                }
            }
        }
        return {true,0,0};
    }



public:


    // Public member function that reads the incomplete puzzle
    // we are not doing any checks on the input puzzle -- that is,
    // we are assuming they are indeed valid
    void read_puzzle(int argc, char * const argv[])
    {

        // write code that reads the input puzzle using the
        // guidelines of figure 23 of the bootcamp material
        int value;
        ifstream input_file(argv[1]);
//        ifstream input_file("input5.txt");
        if (input_file.is_open()){
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    input_file >> value;
                    puzzle[i][j] = value;
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

    bool isPuzzleSloved(int row, int col){
        for(int i = row; i < 9;i++)
            for (int j = col; j < 9; j++)
                if (puzzle[i][j] == 0) return false;
        return true;
    }

    // Public member function that (recursively) implements the brute-force
    // search for possible solutions to the incomplete Sudoku puzzle
    bool Solve(int row, int col)
    {
        if(isPuzzleSloved(row,col)) return true;
        for (int i = 0 ; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (puzzle[i][j]==0){
                    for (int k = 1; k <= 9 ; k++){
                        puzzle[i][j] = k;
                        if (isBoard_valid(i,j) &&
                            get<0>(positive_diagonals_valid()) &&
                            get<0>(adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4()) &&
                            Solve(i,j)
                            ) {
                            return true;
                        }
                    }
                    //cannot slove
                    puzzle[i][j] = 0;
                    return false;
                }
            }
        }
        return false;

        // this part of the code identifies the row and col number of the
        // first incomplete (i.e. 0) entry in the puzzle.  If the puzzle has
        // no zeros, the variable row will be 9 => the puzzle is done, as
        // each entry is row-, col- and block-valid...

        // use the pseudo code of figure 3 of the description
    }
};

#endif