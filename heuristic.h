#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Board.h"
#include "State.h"
#include <array>

long value_heuristic(const State &, const Player &);
long marker_heuristic(const State &, const Player &);
long ring_moves_heuristic(const State &, const Player &);
long ring_connected_heuristic(const State &, const Player &);
long ring_controlled_heuristic(const State &, const Player &);
long ring_heuristic(const std::array<long, 3> &, const State &, const Player &);
#endif