#ifndef BOARD_H
#define BOARD_H

#include "State.h"
#include <boost/bimap.hpp>
#include <vector>

using namespace std;

class Board {
  private:
    typedef boost::bimap<pair<int, int>, pair<int, int>> bm_type;
    typedef bm_type::value_type bm_value_type;

  public:
    State state;
    Player player;
    bm_type bm;

    Board(Player);
    Proper_Ply bestply(int);
    void input_parse(string s, Player);
    pair<int, int> hex_to_coord(pair<int, int>);
    pair<int, int> coord_to_hex(pair<int, int>);
    bool is_game_over() const;
    void checkfor5();
    void printboard();
};

vector<Proper_Ply> generate_plies(const State &state, Player player);
State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform);
string output_parse(const Proper_Ply &);
#endif