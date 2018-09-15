#include "State.h"
#include <cmath>
#include <utility>
#include <algorithm>
#include <iterator>

State::State()
{
    for (auto x = -5; x != 6; ++x)
    {
        for (auto y = -5; y != 6; ++y)
        {
            if (abs(x) == 5 && abs(y) == 5)
                continue;
            board_map[make_pair(x, y)] = EMPTY;
        }
    }
}