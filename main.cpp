#include "Board.h"
#include "State.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int player_id, board_size, time_limit_in_seconds;
    string input_move;
    cin >> player_id >> board_size >> time_limit_in_seconds;
    if (player_id == 2) {
        //other person is moving first
        getline(cin, input_move);
        input_parse(input_move);
    } else if (player_id == 1) {
        //your move
    }
    return 0;
}