//
// Created by job on 5/9/2565.
//

#ifndef N_queens
#define N_queens
#include <set>
using namespace std;

class Board{
    int size;
    int **chess_board;

    set<int> diagUpRight; // nrow+ncol = const
    set<int> diagDownRight; // nrow-ncol = const
    set<int> used_row;

    bool is_this_position_safe(int row,int col){
        if (used_row.find(row) != used_row.end() ||
                diagUpRight.find(row+col)!=diagUpRight.end()||
                diagDownRight.find(row-col)!=diagDownRight.end()) return false;
        used_row.insert(row);
        diagUpRight.insert(row+col);
        diagDownRight.insert(row-col);
        return true;
    }


    void initialize(int n){
        size = n;
        //Initialize chess board
    }
    void print_board(){
        //print board by iterate the array here
    }
    bool solve(int col){

    }

public:
    void nQueens(int n){//use backtracking here
        initialize(n);
        if(solve(0)) print_board();
        else cout<<"There is no solution to the " << n <<" Queens Problem"<< endl;
    }
};

#endif //N_queens
