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
  Ply bestply();
  State input_parse(string s);
  void printboard();
};

#endif