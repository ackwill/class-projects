//
// Created by Will Pittman on 12/12/15.
//

#include "TicTacToe.h"

#define WIDTH 3
#define HEIGHT 3

using namespace std;

TicTacToe::TicTacToe() {
    for(int w = 0; w < WIDTH; w++) {
        for(int h = 0; h < HEIGHT; h++) {
            board[w][h] = ' ';
        }
    }
}

void TicTacToe::print_board() {
    cout << " 1 | 2 | 3 " << endl;
    cout << " " << board[0][0] << " | " << board[1][0] << " | " << board[2][0] << endl;
    cout << "___|___|___" << endl;
    cout << " 4 | 5 | 6 " << endl;
    cout << " " << board[0][1] << " | " << board[1][1] << " | " << board[2][1] << endl;
    cout << "___|___|___" << endl;
    cout << " 7 | 8 | 9 " << endl;
    cout << " " << board[0][2] << " | " << board[1][2] << " | " << board[2][2] << endl;
    cout << "   |   |   " << endl;
}

char TicTacToe::check_win(char player_c) {
            //Vert win
    if((board[0][0] == player_c && board[0][1] == player_c && board[0][2] == player_c) ||
                (board[1][0] == player_c && board[1][1] == player_c && board[1][2] == player_c) ||
                (board[2][0] == player_c && board[2][1] == player_c && board[2][2] == player_c) ||
            //Horiz Win
                (board[0][0] == player_c && board[1][0] == player_c && board[2][0] == player_c) ||
                (board[0][1] == player_c && board[1][1] == player_c && board[2][1] == player_c) ||
                (board[0][2] == player_c && board[1][2] == player_c && board[2][2] == player_c) ||
            //Diag
                (board[0][0] == player_c && board[1][1] == player_c && board[2][2] == player_c) ||
                (board[2][0] == player_c && board[1][1] == player_c && board[0][2] == player_c)) {
        return player_c;
    }

    return ' ';
}

bool TicTacToe::check_tie() {
    for(int w = 0; w < WIDTH; w++) {
        for(int h = 0; h < HEIGHT; h++) {
            if(board[w][h] == ' ') return false;
        }
    }
    return true;
}

bool TicTacToe::is_space_empty(int quadrant, char player_c) {

    switch (quadrant) {
        case 1:
            if(board[0][0] == ' ') {
                board[0][0] = player_c;
                return true;
            }
            break;
        case 2:
            if(board[1][0] == ' ') {
                board[1][0] = player_c;
                return true;
            }
            break;
        case 3:
            if(board[2][0] == ' ') {
                board[2][0] = player_c;
                return true;
            }
            break;
        case 4:
            if(board[0][1] == ' ') {
                board[0][1] = player_c;
                return true;
            }
            break;
        case 5:
            if(board[1][1] == ' ') {
                board[1][1] = player_c;
                return true;
            }
            break;
        case 6:
            if(board[2][1] == ' ') {
                board[2][1] = player_c;
                return true;
            }
            break;
        case 7:
            if(board[0][2] == ' ') {
                board[0][2] = player_c;
                return true;
            }
            break;
        case 8:
            if(board[1][2] == ' ') {
                board[1][2] = player_c;
                return true;
            }
            break;
        case 9:
            if(board[2][2] == ' ') {
                board[2][2] = player_c;
                return true;
            }
            break;
    }
    return false;
}

void TicTacToe::do_turn(char player_c) {
    bool go = true;
    int place;
    while (go) {
        cout << "Enter the quadrant you would like to play in: ";
        cin >> place;
        go = !is_space_empty(place, player_c);
    }
}

void TicTacToe::display_winner(char winner) {
    print_board();
    if(winner == ' ') {
        cout << "Tied!! Play again." << endl;
    } else {
        cout << winner << " won!!" << endl;
    }
}

void TicTacToe::play() {
    while(1) {
        //Print Board
        print_board();
        //Player 1 turn
        do_turn('X');
        //Check winner
        if(check_win('X') != ' ') {
            display_winner('X');
            break;
        }
        if(check_tie()) {
            display_winner(' ');
            break;
        }
        //Print Board
        print_board();
        //Player 2 turn
        do_turn('O');
        //Check winner
        if(check_win('O') != ' ') {
            display_winner('O');
            break;
        }
    }
}