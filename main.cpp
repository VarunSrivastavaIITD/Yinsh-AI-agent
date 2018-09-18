#include "Board.h"
#include "State.h"
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    Player player = WHITE;
    Board game_board(player);
    int player_id, board_size, time_limit_in_seconds;
    string input_move;
    int depth = 3;
    auto begin = chrono::high_resolution_clock::now();
    auto current = chrono::high_resolution_clock::now();

    cin >> player_id >> board_size >> time_limit_in_seconds;
    cin.clear();
    cin.ignore(1000, '\n');

    if (player_id == 2) {
        //other person is moving first
        auto seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();

        while (getline(cin, input_move) && (!game_board.is_game_over()) && (seconds < time_limit_in_seconds)) {
            // boost::trim(input_move);
            game_board.input_parse(input_move, player);
            game_board.checkfor5();
            auto ply = game_board.bestply(depth);
            game_board.state = perform_proper_ply(game_board.state, player, ply);
            // game_board.checkfor5();
            auto output = game_board.output_parse(ply);
            cout << output << endl;
            ;

            current = chrono::high_resolution_clock::now();
            seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();
        }
    } else if (player_id == 1) {
        auto seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();
        auto ply = game_board.bestply(depth);
        game_board.state = perform_proper_ply(game_board.state, player, ply);
        // game_board.checkfor5();
        auto output = game_board.output_parse(ply);
        cout << output << endl;

        while (getline(cin, input_move) && (!game_board.is_game_over()) && (seconds < time_limit_in_seconds)) {
            //boost::trim(input_move);
            game_board.input_parse(input_move, player);
            game_board.checkfor5();
            auto ply = game_board.bestply(depth);
            game_board.state = perform_proper_ply(game_board.state, player, ply);
            // game_board.checkfor5();
            auto output = game_board.output_parse(ply);
            cout << output << endl;

            current = chrono::high_resolution_clock::now();
            seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();
        }
    }
    return 0;
}