#include <iostream>
#include "N_queens.h"
int main(int argc, char * const argv[]) {
    Board x;
    int board_size;
    cin >> board_size;
//    sscanf (argv[1], "%d",&board_size);
    x.nQueens(board_size);
    return 0;
}
