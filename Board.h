#ifndef BOARD_H
#define BOARD_H

#include "State.h"
#include <vector>
using namespace std;

enum Player {
    BLACK,
    WHITE
};

class Board {
  public:
    State state;
    Player player;
    Ply bestply();
    State input_parse(string s, const State &state, const Player &player);
    vector<Proper_Ply> generate_plies(const State &state, Player player);
    void printboard();
};

vector<Proper_Ply> generate_plies(const State &state, Player player);

#endif