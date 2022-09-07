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
        return !(used_row.find(row) != used_row.end() ||
                 diagUpRight.find(row+col)!=diagUpRight.end()||
                 diagDownRight.find(row-col)!=diagDownRight.end());
    }

    void initialize(int n){
        size = n;
        //Initialize chess board
        chess_board = new int*[n]; // pointer pint to n int 1 dim
        for(int i = 0; i < size; i++)
            chess_board[i] = new int[size]; // pointer poiont to n int dim 2 will be 0 when created
    }

    void print_board(){
        //print board by iterate the array here
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++)
                cout << " " << chess_board[i][j]<<" ";
            cout << endl;
        }
    }

    void solve(int col){
        if (col>=size) {
            print_board();
            cout << endl;
            return;
        }
        else{
            for (int row = 0; row<size;row++){ //row
                if (is_this_position_safe(row,col)) continue;
                chess_board[row][col] = 1;
                used_row.insert(row);
                diagUpRight.insert(row+col);
                diagDownRight.insert(row-col);
                solve(col+1);
                //try queen in another position
                chess_board[row][col] = 0;
                used_row.erase(row);
                diagUpRight.erase(row+col);
                diagDownRight.erase(row-col);
                }
            }
        return;
    }

public:
    void nQueens(int n){//use backtracking here
        initialize(n);
        solve(0);
        //else cout<<"There is no solution to the " << n <<" Queens Problem"<< endl;
    }
};

#endif //N_queens
