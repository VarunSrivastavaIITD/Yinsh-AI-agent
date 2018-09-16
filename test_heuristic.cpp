#include <iostream>
using namespace std;
#include "Board.h"
#include "State.h"
#include "heuristic.h"

int main() {
    //cout << "hello"<<endl;
    State s;
    s.board_map[make_pair(0, 0)] = WHITE_RING;
    s.board_map[make_pair(-1, 0)] = BLACK_RING;
    s.board_map[make_pair(1, 1)] = WHITE_RING;
    s.board_map[make_pair(0, -1)] = BLACK_RING;
    s.board_map[make_pair(1, -1)] = WHITE_RING;
    s.board_map[make_pair(0, 1)] = BLACK_RING;
    s.board_map[make_pair(3, 2)] = WHITE_RING;
    s.board_map[make_pair(1, 0)] = BLACK_RING;
    s.board_map[make_pair(2, -1)] = WHITE_RING;
    s.board_map[make_pair(-1, 1)] = BLACK_RING;
    s.white_rings.insert(make_pair(0, 0));
    s.black_rings.insert(make_pair(-1, 0));
    s.white_rings.insert(make_pair(1, 1));
    s.black_rings.insert(make_pair(0, -1));
    s.white_rings.insert(make_pair(1, -1));
    s.black_rings.insert(make_pair(0, 1));
    s.white_rings.insert(make_pair(3, 2));
    s.black_rings.insert(make_pair(1, 0));
    s.white_rings.insert(make_pair(2, -1));
    s.black_rings.insert(make_pair(-1, 1));

    s.mode = S;

    std::cout << ring_controlled_heuristic(s, WHITE) << std::endl;

    return 0;
}