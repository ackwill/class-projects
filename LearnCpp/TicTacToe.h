//
// Created by Will Pittman on 12/12/15.
//

#include <iostream>
#include <vector>

#ifndef TICTACTOE_TICTACTOE_H
#define TICTACTOE_TICTACTOE_H


class TicTacToe {
    char board[3][3];
    char check_win(char player_c);

    bool is_space_empty(int quadrant, char player_c);
    bool check_tie();

    void do_turn(char player_c);
    void print_board();
    void display_winner(char winner);
public:
    TicTacToe();
    void play();
};


#endif //TICTACTOE_TICTACTOE_H
