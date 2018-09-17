#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Board.h"
#include "State.h"
#include <array>

long value_heuristic(const State &, const Player &);
long marker_heuristic(const State &, const Player &);
long ring_moves_heuristic(State &, const Player &);
long ring_connected_heuristic(State &, const Player &);
long ring_controlled_heuristic(State &, const Player &);
long ring_fuse_heuristic(State &, const Player &, long, long);
long ring_heuristic(const std::array<long, 3> &, State &, const Player &);
#endif