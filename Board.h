
#ifndef BOARD_H
#define BOARD_H

#include "State.h"
#include <vector>
using namespace std;

enum Player
{
  BLACK,
  WHITE
};

class Board
{
private:
  State state;
  Player player;

public:
  vector<Ply> generate_plies(const State &, const Player);
  State perform_ply(const State &, Player, Ply);
};

#endif