#ifndef STATE_H
#define STATE_H

#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include <set>

using namespace std;

enum Values
{
  EMPTY,
  WHITE_RING,
  BLACK_RING,
  WHITE_MARKER,
  BLACK_MARKER
};

enum Mode
{
  P,
  S
};

typedef pair<pair<int, int>, pair<int, int>> Ply;
typedef tuple<pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>> Proper_Ply;

class State
{
public:
  Mode mode;
  map<pair<int, int>, Values> board_map;
  set<pair<int, int>> black_rings;
  set<pair<int, int>> white_rings;
  set<pair<int, int>> white_markers;
  set<pair<int, int>> black_markers;

  State();
  static pair<int, int> hex_to_ycoord(pair<size_t, size_t>);
  static pair<size_t, size_t> ycoord_to_hex(pair<int, int>);
};

#endif