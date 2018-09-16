#ifndef BOARD_H
#define BOARD_H

#include "State.h"
#include <boost/bimap.hpp>
#include <vector>

using namespace std;

class Board {
  private:
    typedef boost::bimap<pair<int, int>, pair<int, int>> bm_type;
    typedef bm_type::value_type hex_coord;

  public:
    State state;
    Player player;
    bm_type bm;

    Board();
    Ply bestply();
    State input_parse(string s, const State &state, const Player &player);
    vector<Proper_Ply> generate_plies(const State &state, Player player);
    void printboard();
};

vector<Proper_Ply> generate_plies(const State &state, Player player);

#endif