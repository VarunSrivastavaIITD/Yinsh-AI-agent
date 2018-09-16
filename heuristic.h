#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "State.h"
#include "Board.h"

long value_heuristic(const State &, const Player &);
long marker_heuristic(const State &, const Player &);
long ring_heuristic(const State &, const Player &);
long heuristic(const State &, const Player &);
#endif