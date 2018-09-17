#include "Board.h"
#include "State.h"
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    Board game_board;
    int player_id, board_size, time_limit_in_seconds;
    Player player = WHITE;
    auto begin = chrono::high_resolution_clock::now();
    string input_move;

    cin >> player_id >> board_size >> time_limit_in_seconds;

    if (player_id == 2) {
        //other person is moving first
        getline(cin, input_move);
        boost::trim(input_move);
        game_board.input_parse(input_move, player);
    } else if (player_id == 1) {
        //your move
    }
    return 0;
}