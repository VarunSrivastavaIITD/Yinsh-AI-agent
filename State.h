#ifndef STATE_H
#define STATE_H

#include <boost/functional/hash.hpp>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

enum Player {
    BLACK,
    WHITE
};

enum Values { EMPTY,
              WHITE_RING,
              BLACK_RING,
              WHITE_MARKER,
              BLACK_MARKER };

enum Mode { P,
            S };

typedef pair<pair<int, int>, pair<int, int>> Ply;
typedef tuple<pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>> Proper_Ply;

class State {
  public:
    // typedefs
    typedef pair<int, int> Coordinate;
    // member variables
    Mode mode;
    // unordered_map<Coordinate, Values, boost::hash<Coordinate>>
    //     board_map;
    // unordered_set<Coordinate, boost::hash<Coordinate>> black_rings;
    // unordered_set<Coordinate, boost::hash<Coordinate>> white_rings;
    // unordered_set<Coordinate, boost::hash<Coordinate>> white_markers;
    // unordered_set<Coordinate, boost::hash<Coordinate>> black_markers;
    // unordered_set<
    //     unordered_set<Coordinate, boost::hash<Coordinate>>,
    //     boost::hash<unordered_set<Coordinate, boost::hash<Coordinate>>>>
    //     white_lines;
    // unordered_set<
    //     unordered_set<Coordinate, boost::hash<Coordinate>>,
    //     boost::hash<unordered_set<Coordinate, boost::hash<Coordinate>>>>
    //     black_lines;

    map<Coordinate, Values> board_map;
    set<Coordinate> black_rings;
    set<Coordinate> white_rings;
    set<Coordinate> white_markers;
    set<Coordinate> black_markers;
    // constructor
    State();
    // coordinate converters
    void use_lines() const;

    // non const getters
    decltype(white_rings) &get_player_rings(const Player &);
    decltype(white_markers) &get_player_markers(const Player &);
    decltype(white_rings) &get_other_player_rings(const Player &);
    decltype(white_markers) &get_other_player_markers(const Player &);

    // const getters
    const decltype(white_rings) &get_player_rings(const Player &) const;
    const decltype(white_markers) &get_player_markers(const Player &) const;
    const decltype(white_rings) &get_other_player_rings(const Player &) const;
    const decltype(white_markers) &get_other_player_markers(const Player &) const;
};

#endif