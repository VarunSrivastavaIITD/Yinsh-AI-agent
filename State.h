#ifndef STATE_H
#define STATE_H

#include <vector>
#include <map>
#include <utility>

using namespace std;

enum Values
{
  EMPTY,
  WHITE_RING,
  BLACK_RING,
  WHITE_MARKER,
  BLACK_MARKER
};

class State
{
private:
  map<pair<int, int>, Values> board_map;
  vector<pair<int, int>> white_rings;
  vector<pair<int, int>> black_rings;
  vector<pair<int, int>> white_markers;
  vector<pair<int, int>> black_markers;

public:
  State();
  static pair<int, int> hex_to_ycoord(pair<size_t, size_t>);
  static pair<size_t, size_t> ycoord_to_hex(pair<int, int>);
};

#endif