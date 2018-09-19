#include "Board.h"
#include "State.h"
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // ifstream fin("input5.txt");
    auto &fin = cin;
    Player player = WHITE;
    Board game_board(player);
    int player_id, board_size, time_limit_in_seconds;
    string input_move;
    int depth = 3;
    auto begin = chrono::high_resolution_clock::now();
    auto current = chrono::high_resolution_clock::now();

    fin >> player_id >> board_size >> time_limit_in_seconds;
    fin.clear();
    fin.ignore(1000, '\n');

    if (player_id == 2) {
        //other person is moving first
        auto seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();

        while (getline(fin, input_move) && (!game_board.is_game_over()) && (seconds < time_limit_in_seconds)) {
            auto strx = game_board.remove_other_markers(input_move);
            game_board.input_parse(strx, BLACK);
            // cout << "INPUT MOVE " << input_move << endl;
            // cout << "INPUT PARSE OUTPUT" << endl;
            // print_state(game_board.state);

            auto input_remove_string = game_board.check_for_consecutive_markers();
            // print_map(game_board.state.board_map);
            if (game_board.is_game_over()) {
                cout << input_remove_string << endl;
                break;
            }

            auto ply = game_board.bestply(depth);
            game_board.state = perform_proper_ply(game_board.state, player, ply);
            auto output_remove_string = game_board.check_for_consecutive_markers();

            auto output = game_board.output_parse(ply);

            if (input_remove_string.size() > 0)
                output = input_remove_string + " " + output;
            if (output_remove_string.size() > 0)
                output = output + " " + output_remove_string;

            cout << output << endl;

            // cout << endl
            //      << "OUTPUT STATE" << endl;
            // print_map(game_board.state.board_map);

            current = chrono::high_resolution_clock::now();
            seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();
        }
    } else if (player_id == 1) {
        auto seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();

        auto ply = game_board.bestply(depth);
        game_board.state = perform_proper_ply(game_board.state, player, ply);
        auto output = game_board.output_parse(ply);
        cout << output << endl;

        while (getline(fin, input_move) && (!game_board.is_game_over()) && (seconds < time_limit_in_seconds)) {
            auto strx = game_board.remove_other_markers(input_move);
            game_board.input_parse(strx, BLACK);
            auto input_remove_string = game_board.check_for_consecutive_markers();

            if (game_board.is_game_over()) {
                cout << input_remove_string << endl;
                break;
            }

            auto ply = game_board.bestply(depth);
            game_board.state = perform_proper_ply(game_board.state, player, ply);
            auto output = game_board.output_parse(ply);
            auto output_remove_string = game_board.check_for_consecutive_markers();
            if (input_remove_string.size() > 0)
                output = input_remove_string + " " + output;
            if (output_remove_string.size() > 0)
                output = output + " " + output_remove_string;
            cout << output << endl;

            current = chrono::high_resolution_clock::now();
            seconds = chrono::duration_cast<chrono::seconds>(current - begin).count();
        }
    }
    return 0;
}