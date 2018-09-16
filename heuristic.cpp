#include "heuristic.h"
#include "State.h"
#include <algorithm>
#include <boost/function.hpp>
#include <boost/functional/hash.hpp>
#include <boost/range/combine.hpp>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

const int NUM_RINGS = 5;
typedef long Heuristic(const State &state, const Player &player);

long value_heuristic(const State &state, const Player &player) {
    auto rings = state.get_player_rings(player);
    return 10000 * static_cast<long>(NUM_RINGS - rings.size());
}

long marker_heuristic(const State &state, const Player &player) {
    auto markers = state.get_player_markers(player);
    return 10 * static_cast<long>(markers.size());
}

long ring_moves_heuristic(const State &state, const Player &player) {

    auto add_ply = [](decltype(State::board_map) &bmap, const pair<int, int> p, long &count) -> bool {
        switch (bmap[p]) {
        case EMPTY:
            ++count;
            break;
        case WHITE_RING:
        case BLACK_RING:
            return true;
        case WHITE_MARKER:
        case BLACK_MARKER:
            return false;
        }
        return false;
    };

    auto bmap = state.board_map;
    auto rings = state.get_player_rings(player);
    long count = 0;
    for (const auto &coordinate : rings) {
        for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y) {
            if (add_ply(bmap, make_pair(coordinate.first, y), count))
                break;
        }

        for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y) {
            if (add_ply(bmap, make_pair(coordinate.first, y), count))
                break;
        }

        for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x) {
            if (add_ply(bmap, make_pair(x, coordinate.second), count))
                break;
        }

        for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x) {
            if (add_ply(bmap, make_pair(x, coordinate.second), count))
                break;
        }

        for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y) {
            if (add_ply(bmap, make_pair(x, y), count))
                break;
        }

        for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y) {
            if (add_ply(bmap, make_pair(x, y), count))
                break;
        }
    }
    return count;
}

long ring_connected_heuristic(const State &state, const Player &player) {
    // auto is_connected = [](const State::Coordinate& p1, const State::Coordinate& p2) -> bool {
    //     return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
    // };

    std::vector<State::Coordinate> coords;
    for (const auto &m : state.board_map)
        coords.push_back(m.first);

    auto rings = state.get_player_rings(player);

    long count = 0;
    for (const auto &r : rings)
        count += std::count_if(coords.begin(), coords.end(), [&r](const State::Coordinate &x) {
            return r.first == x.first || r.second == x.second || (r.second - r.first) == (x.second - x.first);
        });

    return count;
}

long ring_controlled_heuristic(const State &state, const Player &player) {
    return 0;
}

boost::function<long(const State &state, const Player &player)> heuristic_combinator(const vector<long> &weights, const vector<Heuristic> &heuristics) {
    auto f = [&](const State &state, const Player &player) -> long {
        long res = 0;
        for (const auto &tup : boost::combine(weights, heuristics)) {
            long w;
            Heuristic h;

            boost::tie(w, h) = tup;
            res += w * h(state, player);
        }
    };

    boost::function<long(const State &state, const Player &player)> return_func = f;
    return return_func;
}