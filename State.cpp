#include "State.h"
#include "Board.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <utility>

State::State() {
    for (auto x = -5; x != 6; ++x) {
        for (auto y = -5; y != 6; ++y) {
            if ((abs(x) == 5 && abs(y) == 5)||(abs(x) == 5 && abs(y) == 0)||(abs(x) == 0 && abs(y) == 5)||(abs(x-y)>5)||(abs(y-x)>5))
                continue;
            board_map[make_pair(x, y)] = EMPTY;
        }
    }
}

decltype(State::white_markers) &State::get_player_markers(const Player &player) {
    switch (player) {
    case WHITE:
        return white_markers;
    case BLACK:
        return black_markers;
    }
}

decltype(State::white_rings) &State::get_other_player_rings(const Player &player) {
    switch (player) {
    case WHITE:
        return black_rings;
    case BLACK:
        return white_rings;
    }
}

decltype(State::white_markers) &State::get_other_player_markers(const Player &player) {
    switch (player) {
    case WHITE:
        return black_markers;
    case BLACK:
        return white_markers;
    }
}

decltype(State::white_rings) &State::get_player_rings(const Player &player) {
    switch (player) {
    case WHITE:
        return white_rings;
    case BLACK:
        return black_rings;
    }
}

const decltype(State::white_markers) &State::get_player_markers(const Player &player) const {
    switch (player) {
    case WHITE:
        return white_markers;
    case BLACK:
        return black_markers;
    }
}

const decltype(State::white_rings) &State::get_other_player_rings(const Player &player) const {
    switch (player) {
    case WHITE:
        return black_rings;
    case BLACK:
        return white_rings;
    }
}

const decltype(State::white_markers) &State::get_other_player_markers(const Player &player) const {
    switch (player) {
    case WHITE:
        return black_markers;
    case BLACK:
        return white_markers;
    }
}

const decltype(State::white_rings) &State::get_player_rings(const Player &player) const {
    switch (player) {
    case WHITE:
        return white_rings;
    case BLACK:
        return black_rings;
    }
}
