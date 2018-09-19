#include "Board.h"
#include "State.h"
#include "heuristic.h"
#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

auto const Placing = 10;
auto const Number_of_rings = 5;
auto const K = 5;

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(const State &state, Player player, vector<Ply> &p);
State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform);
vector<Proper_Ply> generate_pmode_plies(const State &state);
double negascout(State &state, double alpha, double beta, int depth, Player player);
void print_state(const State &);

void print_map(decltype(State::board_map) bmap) {
    cout << endl
         << "MAP START" << endl;

    cout << "Num Elements" << bmap.size() << endl;
    for (const auto &e : bmap) {
        cout << "Pair: " << e.first.first << " " << e.first.second
             << " Element: " << e.second << endl;
    }
    cout << "MAP END" << endl;
}

Board::Board(Player p) {
    player = p;
    state.mode = P;
    // hex 0
    bm.insert(bm_value_type(make_pair(0, 0), make_pair(0, 0)));
    // hex 1
    bm.insert(bm_value_type(make_pair(1, 0), make_pair(0, 1)));
    bm.insert(bm_value_type(make_pair(1, 1), make_pair(1, 1)));
    bm.insert(bm_value_type(make_pair(1, 2), make_pair(1, 0)));
    bm.insert(bm_value_type(make_pair(1, 3), make_pair(0, -1)));
    bm.insert(bm_value_type(make_pair(1, 4), make_pair(-1, -1)));
    bm.insert(bm_value_type(make_pair(1, 5), make_pair(-1, 0)));
    // hex 2
    bm.insert(bm_value_type(make_pair(2, 0), make_pair(0, 2)));
    bm.insert(bm_value_type(make_pair(2, 1), make_pair(1, 2)));
    bm.insert(bm_value_type(make_pair(2, 2), make_pair(2, 2)));
    bm.insert(bm_value_type(make_pair(2, 3), make_pair(2, 1)));
    bm.insert(bm_value_type(make_pair(2, 4), make_pair(2, 0)));
    bm.insert(bm_value_type(make_pair(2, 5), make_pair(1, -1)));
    bm.insert(bm_value_type(make_pair(2, 6), make_pair(0, -2)));
    bm.insert(bm_value_type(make_pair(2, 7), make_pair(-1, -2)));
    bm.insert(bm_value_type(make_pair(2, 8), make_pair(-2, -2)));
    bm.insert(bm_value_type(make_pair(2, 9), make_pair(-2, -1)));
    bm.insert(bm_value_type(make_pair(2, 10), make_pair(-2, 0)));
    bm.insert(bm_value_type(make_pair(2, 11), make_pair(-1, 1)));
    // hex 3
    bm.insert(bm_value_type(make_pair(3, 0), make_pair(0, 3)));
    bm.insert(bm_value_type(make_pair(3, 1), make_pair(1, 3)));
    bm.insert(bm_value_type(make_pair(3, 2), make_pair(2, 3)));
    bm.insert(bm_value_type(make_pair(3, 3), make_pair(3, 3)));
    bm.insert(bm_value_type(make_pair(3, 4), make_pair(3, 2)));
    bm.insert(bm_value_type(make_pair(3, 5), make_pair(3, 1)));
    bm.insert(bm_value_type(make_pair(3, 6), make_pair(3, 0)));
    bm.insert(bm_value_type(make_pair(3, 7), make_pair(2, -1)));
    bm.insert(bm_value_type(make_pair(3, 8), make_pair(1, -2)));
    bm.insert(bm_value_type(make_pair(3, 9), make_pair(0, -3)));
    bm.insert(bm_value_type(make_pair(3, 10), make_pair(-1, -3)));
    bm.insert(bm_value_type(make_pair(3, 11), make_pair(-2, -3)));
    bm.insert(bm_value_type(make_pair(3, 12), make_pair(-3, -3)));
    bm.insert(bm_value_type(make_pair(3, 13), make_pair(-3, -2)));
    bm.insert(bm_value_type(make_pair(3, 14), make_pair(-3, -1)));
    bm.insert(bm_value_type(make_pair(3, 15), make_pair(-3, 0)));
    bm.insert(bm_value_type(make_pair(3, 16), make_pair(-2, 1)));
    bm.insert(bm_value_type(make_pair(3, 17), make_pair(-1, 2)));
    // hex 4
    bm.insert(bm_value_type(make_pair(4, 0), make_pair(0, 4)));
    bm.insert(bm_value_type(make_pair(4, 1), make_pair(1, 4)));
    bm.insert(bm_value_type(make_pair(4, 2), make_pair(2, 4)));
    bm.insert(bm_value_type(make_pair(4, 3), make_pair(3, 4)));
    bm.insert(bm_value_type(make_pair(4, 4), make_pair(4, 4)));
    bm.insert(bm_value_type(make_pair(4, 5), make_pair(4, 3)));
    bm.insert(bm_value_type(make_pair(4, 6), make_pair(4, 2)));
    bm.insert(bm_value_type(make_pair(4, 7), make_pair(4, 1)));
    bm.insert(bm_value_type(make_pair(4, 8), make_pair(4, 0)));
    bm.insert(bm_value_type(make_pair(4, 9), make_pair(3, -1)));
    bm.insert(bm_value_type(make_pair(4, 10), make_pair(2, -2)));
    bm.insert(bm_value_type(make_pair(4, 11), make_pair(1, -3)));
    bm.insert(bm_value_type(make_pair(4, 12), make_pair(0, -4)));
    bm.insert(bm_value_type(make_pair(4, 13), make_pair(-1, -4)));
    bm.insert(bm_value_type(make_pair(4, 14), make_pair(-2, -4)));
    bm.insert(bm_value_type(make_pair(4, 15), make_pair(-3, -4)));
    bm.insert(bm_value_type(make_pair(4, 16), make_pair(-4, -4)));
    bm.insert(bm_value_type(make_pair(4, 17), make_pair(-4, -3)));
    bm.insert(bm_value_type(make_pair(4, 18), make_pair(-4, -2)));
    bm.insert(bm_value_type(make_pair(4, 19), make_pair(-4, -1)));
    bm.insert(bm_value_type(make_pair(4, 20), make_pair(-4, 0)));
    bm.insert(bm_value_type(make_pair(4, 21), make_pair(-3, 1)));
    bm.insert(bm_value_type(make_pair(4, 22), make_pair(-2, 2)));
    bm.insert(bm_value_type(make_pair(4, 23), make_pair(-1, 3)));
    // hex 5
    bm.insert(bm_value_type(make_pair(5, 1), make_pair(1, 5)));
    bm.insert(bm_value_type(make_pair(5, 2), make_pair(2, 5)));
    bm.insert(bm_value_type(make_pair(5, 3), make_pair(3, 5)));
    bm.insert(bm_value_type(make_pair(5, 4), make_pair(4, 5)));
    bm.insert(bm_value_type(make_pair(5, 6), make_pair(5, 4)));
    bm.insert(bm_value_type(make_pair(5, 7), make_pair(5, 3)));
    bm.insert(bm_value_type(make_pair(5, 8), make_pair(5, 2)));
    bm.insert(bm_value_type(make_pair(5, 9), make_pair(5, 1)));
    bm.insert(bm_value_type(make_pair(5, 11), make_pair(4, -1)));
    bm.insert(bm_value_type(make_pair(5, 12), make_pair(3, -2)));
    bm.insert(bm_value_type(make_pair(5, 13), make_pair(2, -3)));
    bm.insert(bm_value_type(make_pair(5, 14), make_pair(1, -4)));
    bm.insert(bm_value_type(make_pair(5, 16), make_pair(-1, -5)));
    bm.insert(bm_value_type(make_pair(5, 17), make_pair(-2, -5)));
    bm.insert(bm_value_type(make_pair(5, 18), make_pair(-3, -5)));
    bm.insert(bm_value_type(make_pair(5, 19), make_pair(-4, -5)));
    bm.insert(bm_value_type(make_pair(5, 21), make_pair(-5, -4)));
    bm.insert(bm_value_type(make_pair(5, 22), make_pair(-5, -3)));
    bm.insert(bm_value_type(make_pair(5, 23), make_pair(-5, -2)));
    bm.insert(bm_value_type(make_pair(5, 24), make_pair(-5, -1)));
    bm.insert(bm_value_type(make_pair(5, 26), make_pair(-4, 1)));
    bm.insert(bm_value_type(make_pair(5, 27), make_pair(-3, 2)));
    bm.insert(bm_value_type(make_pair(5, 28), make_pair(-2, 3)));
    bm.insert(bm_value_type(make_pair(5, 29), make_pair(-1, 4)));
}

Proper_Ply Board::bestply(int depth) {
    auto plies = generate_plies(state, player);
    Proper_Ply best_ply;
    Player other_player;
    if (player == WHITE)
        other_player = BLACK;
    else
        other_player = WHITE;
    auto score = numeric_limits<double>::lowest();
    if (state.mode == P)
        depth = 1;

    for (auto it = plies.begin(); it != plies.end(); ++it) {
        auto child = perform_proper_ply(state, player, *it);
        auto s = -negascout(child, -100, 100, depth - 1, other_player);

        if (s > score) {
            score = s;
            best_ply = *it;
        }
    }
    return best_ply;
}

bool is_connected(pair<int, int> p1, pair<int, int> p2) {
    return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
}

bool is_adjacent(pair<int, int> p1, pair<int, int> p2) {
    return (p1.first == p2.first && abs(p1.second - p2.second) == 1) ||
           (p1.second == p2.second && abs(p1.first - p2.first) == 1) ||
           (abs(p2.second - p1.second) == 1 && abs(p2.first - p1.first) == 1);
}

bool add_ply(decltype(State::board_map) &bmap, const pair<int, int> p, vector<Ply> &vec, const pair<int, int> coordinate, bool &found_marker) {

    switch (bmap[p]) {
    case EMPTY:
        vec.emplace_back(coordinate, p);
        if (!found_marker) {
            return false;
            break;
        } else {
            return true;
        }
    case WHITE_RING:
    case BLACK_RING:
        return true;
    case WHITE_MARKER:
    case BLACK_MARKER:
        found_marker = true;
        return false;
    }
    return false;
}

State Toggle(pair<int, int> p2, pair<int, int> p3, const State &state) {
    State newstate = state;
    auto flip = [](decltype(newstate.black_markers) &s, decltype(newstate.black_markers) &t, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
        t.insert(e);
    };
    if (p2.first == p3.first) {
        if (p3.second > p2.second) {
            for (int i = p2.second + 1; i < p3.second; i++) {
                if (newstate.board_map[make_pair(p2.first, i)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(p2.first, i)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(p2.first, i));
                } else if (newstate.board_map[make_pair(p2.first, i)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(p2.first, i)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(p2.first, i));
                }
            }
        }

        else {
            for (int i = p2.second - 1; i > p3.second; i--) {
                if (newstate.board_map[make_pair(p2.first, i)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(p2.first, i)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(p2.first, i));
                } else if (newstate.board_map[make_pair(p2.first, i)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(p2.first, i)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(p2.first, i));
                }
            }
        }
    }

    else if (p2.second == p3.second) {
        if (p3.first > p2.first) {
            for (int i = p2.first + 1; i < p3.first; i++) {
                if (newstate.board_map[make_pair(i, p2.second)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(i, p2.second)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, p2.second));
                } else if (newstate.board_map[make_pair(i, p2.second)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(i, p2.second)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, p2.second));
                }
            }
        } else {
            for (int i = p2.first - 1; i > p3.first; i--) {
                if (newstate.board_map[make_pair(i, p2.second)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(i, p2.second)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, p2.second));
                } else if (newstate.board_map[make_pair(i, p2.second)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(i, p2.second)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, p2.second));
                }
            }
        }
    } else {
        if (p3.first > p2.first) {
            for (int i = p2.first + 1, j = p2.second + 1; i < p3.first; i++, j++) {
                if (newstate.board_map[make_pair(i, j)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(i, j)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, j));
                } else if (newstate.board_map[make_pair(i, j)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(i, j)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, j));
                }
            }
        } else {
            for (int i = p2.first - 1, j = p2.second - 1; i > p3.first; i--, j--) {
                if (newstate.board_map[make_pair(i, j)] == WHITE_MARKER) {
                    newstate.board_map[make_pair(i, j)] = BLACK_MARKER;
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, j));
                } else if (newstate.board_map[make_pair(i, j)] == BLACK_MARKER) {
                    newstate.board_map[make_pair(i, j)] = WHITE_MARKER;
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, j));
                }
            }
        }
    }
    return newstate;
}

vector<Proper_Ply> Board::generate_plies(const State &state, Player player) {
    vector<Proper_Ply> proper_plies;
    vector<Ply> plies;
    if (state.mode == P) {
        proper_plies = generate_pmode_plies(state);
    } else {
        auto bmap = state.board_map;
        decltype(state.white_rings) rings;

        if (player == WHITE)
            rings = state.white_rings;
        else
            rings = state.black_rings;

        bool found_marker = false;
        for (const auto &coordinate : rings) {
            for (auto y = coordinate.second + 1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y) {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate, found_marker))
                    break;
            }

            found_marker = false;
            for (auto y = coordinate.second - 1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y) {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate, found_marker))
                    break;
            }
            found_marker = false;

            for (auto x = coordinate.first + 1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x) {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate, found_marker))
                    break;
            }
            found_marker = false;

            for (auto x = coordinate.first - 1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x) {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate, found_marker))
                    break;
            }
            found_marker = false;

            for (auto x = coordinate.first + 1, y = coordinate.second + 1; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y) {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate, found_marker))
                    break;
            }
            found_marker = false;

            for (auto x = coordinate.first - 1, y = coordinate.second - 1; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y) {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate, found_marker))
                    break;
            }
        }
        // cout << plies.size() << "##";
        proper_plies = Generating_proper_moves_from_selection_moves(state, player, plies);
        /*for (auto q = plies.begin(); q != plies.end(); q++)
        {
            cout<<(*q).first.first<<" "<<(*q).first.second<<endl;
            cout<<(*q).second.first<<" "<<(*q).second.second<<endl;
            cout<<endl;
            proper_plies = Generating_proper_moves_from_selection_moves(state, *q, player, proper_plies);
        }*/
    }
    if (state.mode == S) {
        sort(proper_plies.begin(), proper_plies.end(),
             [this](Proper_Ply const &a, Proper_Ply const &b) -> bool {
                 return bm.right.at(get<1>(a)).first < bm.right.at(get<1>(b)).first;
             });
    }
    return proper_plies;
}

double Board::negascout(State &state, double alpha, double beta, int depth, Player player) {
    if (depth == 0) {
        auto h = heuristic(state, player);
        return h;
    }

    Player other_player;
    if (player == WHITE)
        other_player = BLACK;
    else
        other_player = WHITE;

    auto plies = generate_plies(state, player);
    auto returned_alpha = numeric_limits<double>::lowest();

    bool first = true;
    for (const auto &proper_ply : plies) {

        auto nextstate = perform_proper_ply(state, player, proper_ply);

        if (first)
            returned_alpha = -negascout(nextstate, -beta, -alpha, depth - 1, other_player);
        else {
            returned_alpha = -negascout(nextstate, -alpha - 1, -alpha, depth - 1, other_player);
            if (returned_alpha > alpha && returned_alpha < beta)
                returned_alpha = -negascout(nextstate, -beta, -returned_alpha, depth - 1, other_player);
        }
        first = false;
        alpha = max(alpha, returned_alpha);
        if (alpha >= beta)
            return alpha;
    }

    return alpha;
}

pair<int, int> Board::hex_to_coord(pair<int, int> p) {
    return bm.left.at(p);
}

pair<int, int> Board::coord_to_hex(pair<int, int> p) {
    return bm.right.at(p);
}

void Board::input_parse(string s, Player player = BLACK) {
    // player = BLACK;

    istringstream ss(s);
    vector<string> vec;
    copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(vec));

    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    if (vec[0] == "P") {
        pair<int, int> p1 = hex_to_coord(make_pair(stoi(vec[1]), stoi(vec[2])));
        if (player == WHITE) {
            newstate.board_map[p1] = WHITE_RING;
            newstate.white_rings.insert(p1);
        } else {
            newstate.board_map[p1] = BLACK_RING;
            newstate.black_rings.insert(p1);
        }
    } else if (vec[0] == "S") {
        pair<int, int> p2 = hex_to_coord(make_pair(stoi(vec[1]), stoi(vec[2])));
        pair<int, int> p3 = hex_to_coord(make_pair(stoi(vec[4]), stoi(vec[5])));
        decltype(newstate.board_map)::mapped_type ring;
        decltype(newstate.board_map)::mapped_type marker;
        auto rings = std::ref(newstate.white_rings);
        auto markers = std::ref(newstate.white_markers);
        auto othermarkers = std::ref(newstate.black_markers);

        if (player == WHITE) {
            marker = WHITE_MARKER;
            ring = WHITE_RING;
            markers = std::ref(newstate.white_markers);
            othermarkers = std::ref(newstate.black_markers);
            rings = std::ref(newstate.white_rings);
        } else {
            marker = BLACK_MARKER;
            ring = BLACK_RING;
            markers = std::ref(newstate.black_markers);
            othermarkers = std::ref(newstate.white_markers);
            rings = std::ref(newstate.black_rings);
        }
        newstate.board_map[p2] = marker;
        newstate.board_map[p3] = ring;

        auto search = rings.get().find(p2);
        if (search != rings.get().end())
            rings.get().erase(search);

        //cout<<"&&"<<p3.first<<" "<<p3.second<<endl;
        //cout<<"&&"<<newstate.board_map[make_pair(3,  2)]<<endl;
        rings.get().insert(p3);   //add p3 to rings
        markers.get().insert(p2); //add p2 to markers
        //toggle & change white&blackmarker, board_map
        newstate = Toggle(p2, p3, newstate);

        if (vec.size() > 6) {
            size_t k = 6;
            while (k < vec.size()) {
                pair<int, int> p4 = hex_to_coord(make_pair(stoi(vec[k + 1]), stoi(vec[k + 2])));
                pair<int, int> p5 = hex_to_coord(make_pair(stoi(vec[k + 4]), stoi(vec[k + 5])));
                pair<int, int> p6 = hex_to_coord(make_pair(stoi(vec[k + 7]), stoi(vec[k + 8])));
                if (p4.first == p5.first) {
                    if (p5.second > p4.second) {
                        for (int i = p4.second; i <= p5.second; i++) {
                            newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                            delete_from_set(markers, make_pair(p4.first, i));
                        }
                    } else {
                        for (int i = p4.second; i >= p5.second; i--) {
                            newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                            delete_from_set(markers, make_pair(p4.first, i));
                        }
                    }
                } else if (p4.second == p5.second) {
                    if (p5.first > p4.first) {
                        for (int i = p4.first; i <= p5.first; i++) {
                            newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                            delete_from_set(markers, make_pair(i, p4.second));
                        }
                    } else {
                        for (int i = p4.first; i >= p5.first; i--) {
                            newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                            delete_from_set(markers, make_pair(i, p4.second));
                        }
                    }
                } else {
                    if (p5.first > p4.first) {
                        for (int i = p4.first, j = p4.second; i <= p5.first; i++, j++) {
                            newstate.board_map[make_pair(i, j)] = EMPTY;
                            delete_from_set(markers, make_pair(i, j));
                        }
                    } else {
                        for (int i = p4.first, j = p4.second; i >= p5.first; i--, j--) {
                            newstate.board_map[make_pair(i, j)] = EMPTY;
                            delete_from_set(markers, make_pair(i, j));
                        }
                    }
                }
                newstate.board_map[p6] = EMPTY;
                delete_from_set(rings, p6);
                k = k + 9;
            }
        }
    }
    //cout<<"&&"<<newstate.board_map[make_pair(5,  4)]<<endl;
    //cout<<"&&"<<newstate.board_map[make_pair(3,  2)]<<endl;
    state = std::move(newstate);
}

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(const State &state, Player player, vector<Ply> &plies) {

    vector<Proper_Ply> proper_p;
    for (auto q = plies.begin(); q != plies.end(); q++) {
        State newstate = state;
        //cout<<(*q).first.first<<" "<<(*q).first.second<<endl;
        //cout<<(*q).second.first<<" "<<(*q).second.second<<endl;
        //cout<<endl;
        //proper_plies = Generating_proper_moves_from_selection_moves(state, *q, player, proper_plies);
        Ply p = *q;
        decltype(state.board_map)::mapped_type ring;
        decltype(state.board_map)::mapped_type marker;
        auto rings = std::ref(newstate.white_rings);
        auto otherrings = std::ref(newstate.black_rings);
        auto markers = std::ref(newstate.white_markers);
        auto othermarkers = std::ref(newstate.black_markers);

        if (player == WHITE) {
            marker = WHITE_MARKER;
            ring = WHITE_RING;
            markers = std::ref(newstate.white_markers);
            othermarkers = std::ref(newstate.black_markers);
            rings = std::ref(newstate.white_rings);
            otherrings = std::ref(newstate.black_rings);
        } else {
            marker = BLACK_MARKER;
            ring = BLACK_RING;
            markers = std::ref(newstate.black_markers);
            othermarkers = std::ref(newstate.white_markers);
            rings = std::ref(newstate.black_rings);
            otherrings = std::ref(newstate.white_rings);
        }
        // implementing ply before converting to properply
        newstate.board_map[p.first] = marker;
        newstate.board_map[p.second] = ring;
        auto search = rings.get().find(p.first);
        if (search != rings.get().end()) {
            rings.get().erase(search);
            rings.get().insert(p.second);
        }
        markers.get().insert(p.first);
        newstate = Toggle(p.first, p.second, newstate);

        map<pair<int, int>, tuple<int, int, int>> marker_lines;
        for (auto i = markers.get().begin(); i != markers.get().end(); i++) {
            marker_lines[*i] = make_tuple(1, 1, 1);
        }
        auto bmap = newstate.board_map;
        int markers_left;
        int markers_right;
        pair<int, int> left_most;
        int flag = 0;
        for (const auto &coordinate : markers.get()) {
            left_most = coordinate;
            if (get<0>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto y = coordinate.second + 1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y) {
                    if (bmap[make_pair(coordinate.first, y)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<0>(marker_lines[make_pair(coordinate.first, y)]) = 0;
                    }
                }
                //cout<<markers_right<<"R ";
                for (auto y = coordinate.second - 1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y) {
                    if (bmap[make_pair(coordinate.first, y)] != marker)
                        break;
                    else {
                        markers_left++;
                        left_most = make_pair(coordinate.first, y);
                        get<0>(marker_lines[make_pair(coordinate.first, y)]) = 0;
                    }
                }
                //cout<<markers_left<<"L ";
                get<0>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
                if (!(markers_left + markers_right + 1 < 5)) {
                    for (int d = 0; d <= (markers_left + markers_right + 1 - 5); d++) {
                        flag = 1;
                        for (const auto &coordinate2 : rings.get()) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first, left_most.second + d), make_pair(left_most.first, left_most.second + 4 + d), coordinate2);
                        }
                    }
                } else {
                    //cout<<"inelse";
                    //flag += 1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            left_most = coordinate;
            if (get<1>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto x = coordinate.first + 1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x) {
                    if (bmap[make_pair(x, coordinate.second)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<1>(marker_lines[make_pair(x, coordinate.second)]) = 0;
                    }
                }

                for (auto x = coordinate.first - 1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x) {
                    if (bmap[make_pair(x, coordinate.second)] != marker)
                        break;
                    else {
                        markers_left++;
                        left_most = make_pair(x, coordinate.second);
                        get<1>(marker_lines[make_pair(x, coordinate.second)]) = 0;
                    }
                }
                get<1>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
                if (!(markers_left + markers_right + 1 < 5)) {
                    for (int d = 0; d <= (markers_left + markers_right + 1 - 5); d++) {
                        flag = 1;
                        for (const auto &coordinate2 : rings.get()) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first + d, left_most.second), make_pair(left_most.first + 4 + d, left_most.second), coordinate2);
                        }
                    }
                } else {
                    //cout<<"inelse";
                    //flag += 1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            left_most = coordinate;
            if (get<2>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto x = coordinate.first + 1, y = coordinate.second + 1; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y) {
                    if (bmap[make_pair(x, y)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<2>(marker_lines[make_pair(x, y)]) = 0;
                    }
                }

                for (auto x = coordinate.first - 1, y = coordinate.second - 1; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y) {
                    if (bmap[make_pair(x, y)] != marker)
                        break;
                    else {
                        markers_left++;
                        left_most = make_pair(x, y);
                        get<2>(marker_lines[make_pair(x, y)]) = 0;
                    }
                }
                get<2>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
                if (!(markers_left + markers_right + 1 < 5)) {
                    for (int d = 0; d <= (markers_left + markers_right + 1 - 5); d++) {
                        flag = 1;
                        for (const auto &coordinate2 : rings.get()) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first + d, left_most.second + d), make_pair(left_most.first + 4 + d, left_most.second + 4 + d), coordinate2);
                        }
                    }
                } else {
                    //cout<<"inelse";
                    //flag += 1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            //cout<<flag<<"$$";
        }
        if (flag == 0) {
            //cout<<"yay";
            proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
        }
    }
    return proper_p;
}

vector<Proper_Ply> generate_pmode_plies(const State &state) {
    vector<Proper_Ply> proper_p;
    for (auto it = state.board_map.begin(); it != state.board_map.end(); ++it) {
        if (it->second == EMPTY) {
            proper_p.emplace_back(it->first, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
        }
    }
    return proper_p;
}

State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform) {
    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    vector<Proper_Ply> proper_p;
    decltype(state.board_map)::mapped_type ring;
    decltype(state.board_map)::mapped_type marker;
    auto rings = std::ref(newstate.white_rings);
    auto otherrings = std::ref(newstate.black_rings);
    auto markers = std::ref(newstate.white_markers);
    auto othermarkers = std::ref(newstate.black_markers);

    if (player == WHITE) {
        marker = WHITE_MARKER;
        ring = WHITE_RING;
        markers = std::ref(newstate.white_markers);
        othermarkers = std::ref(newstate.black_markers);
        rings = std::ref(newstate.white_rings);
        otherrings = std::ref(newstate.black_rings);
    } else {
        marker = BLACK_MARKER;
        ring = BLACK_RING;
        markers = std::ref(newstate.black_markers);
        othermarkers = std::ref(newstate.white_markers);
        rings = std::ref(newstate.black_rings);
        otherrings = std::ref(newstate.white_rings);
    }
    if (get<1>(proper_ply_toperform) == make_pair(10, 10)) {
        newstate.board_map[get<0>(proper_ply_toperform)] = ring;
        rings.get().insert(get<0>(proper_ply_toperform));
        if (rings.get().size() == Number_of_rings) {
            newstate.mode = S;
        }
        return newstate;
    } else if (get<2>(proper_ply_toperform) == make_pair(10, 10)) {
        //cout << "incorrect";
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.get().find(get<0>(proper_ply_toperform));
        if (search != rings.get().end()) {
            rings.get().erase(search);
            rings.get().insert(get<1>(proper_ply_toperform));
        }
        markers.get().insert(get<0>(proper_ply_toperform));
        newstate = Toggle(get<0>(proper_ply_toperform), get<1>(proper_ply_toperform), newstate);
        return newstate;
    } else {
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.get().find(get<0>(proper_ply_toperform));
        if (search != rings.get().end()) {
            rings.get().erase(search);
            rings.get().insert(get<1>(proper_ply_toperform));
        }
        markers.get().insert(get<0>(proper_ply_toperform));
        newstate = Toggle(get<0>(proper_ply_toperform), get<1>(proper_ply_toperform), newstate);

        pair<int, int> p4 = get<2>(proper_ply_toperform);
        pair<int, int> p5 = get<3>(proper_ply_toperform);
        pair<int, int> p6 = get<4>(proper_ply_toperform);
        if (p4.first == p5.first) {
            if (p5.second > p4.second) {
                for (int i = p4.second; i <= p5.second; i++) {
                    newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            } else {
                for (int i = p4.second; i >= p5.second; i--) {
                    newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            }
        } else if (p4.second == p5.second) {
            if (p5.first > p4.first) {
                for (int i = p4.first; i <= p5.first; i++) {
                    newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            } else {
                for (int i = p4.first; i >= p5.first; i--) {
                    newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            }
        } else {
            if (p5.first > p4.first) {
                for (int i = p4.first, j = p4.second; i <= p5.first; i++, j++) {
                    newstate.board_map[make_pair(i, j)] = EMPTY;
                    delete_from_set(markers, make_pair(i, j));
                }
            } else {
                for (int i = p4.first, j = p4.second; i >= p5.first; i--, j--) {
                    newstate.board_map[make_pair(i, j)] = EMPTY;
                    delete_from_set(markers, make_pair(i, j));
                }
            }
        }
        newstate.board_map[p6] = EMPTY;
        delete_from_set(rings, p6);
    }
    return newstate;
}

bool Isterminal(const State &state) {
    if ((state.mode == S) && (state.white_rings.size() < 3) && ((state.white_rings.size() < 3))) {
        return true;
    }
    return false;
}

bool Board::is_game_over() const {
    if (state.mode != P) {
        return state.black_rings.size() == 2 || state.white_rings.size() == 2;
    }
    return false;
}

string Board::output_parse(Proper_Ply &proper_ply_tooutput) {
    string s;

    if (get<0>(proper_ply_tooutput) != make_pair(10, 10))
        get<0>(proper_ply_tooutput) = coord_to_hex(get<0>(proper_ply_tooutput));
    if (get<1>(proper_ply_tooutput) != make_pair(10, 10))
        get<1>(proper_ply_tooutput) = coord_to_hex(get<1>(proper_ply_tooutput));
    if (get<2>(proper_ply_tooutput) != make_pair(10, 10))
        get<2>(proper_ply_tooutput) = coord_to_hex(get<2>(proper_ply_tooutput));
    if (get<3>(proper_ply_tooutput) != make_pair(10, 10))
        get<3>(proper_ply_tooutput) = coord_to_hex(get<3>(proper_ply_tooutput));
    if (get<4>(proper_ply_tooutput) != make_pair(10, 10))
        get<4>(proper_ply_tooutput) = coord_to_hex(get<4>(proper_ply_tooutput));

    if (get<1>(proper_ply_tooutput) == make_pair(10, 10)) {
        s = "P ";
        s = s + to_string(get<0>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<0>(proper_ply_tooutput).second);
        return s;
        //s[2] = (char)
    } else if (get<2>(proper_ply_tooutput) == make_pair(10, 10)) {
        s = "S ";
        s = s + to_string(get<0>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<0>(proper_ply_tooutput).second);
        s = s + " M ";
        s = s + to_string(get<1>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<1>(proper_ply_tooutput).second);
        return s;
    } else {
        s = "S ";
        s = s + to_string(get<0>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<0>(proper_ply_tooutput).second);
        s = s + " M ";
        s = s + to_string(get<1>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<1>(proper_ply_tooutput).second);
        s = s + " RS ";
        s = s + to_string(get<2>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<2>(proper_ply_tooutput).second);
        s = s + " RE ";
        s = s + to_string(get<3>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<3>(proper_ply_tooutput).second);
        s = s + " X ";
        s = s + to_string(get<4>(proper_ply_tooutput).first);
        s = s + " ";
        s = s + to_string(get<4>(proper_ply_tooutput).second);
        return s;
    }
    return s;
}

void print_state(const State &state) {

    cerr << endl
         << "State start" << endl;

    for (auto w : state.white_rings)
        cerr << "White Rings: " << w.first << " " << w.second << endl;
    cerr << "-----------------------------------------------" << endl;

    for (auto w : state.black_rings)
        cerr << "Black Rings: " << w.first << " " << w.second << endl;
    cerr << "-----------------------------------------------" << endl;

    for (auto w : state.white_markers)
        cerr << "White Markers: " << w.first << " " << w.second << endl;

    cerr << "-----------------------------------------------" << endl;
    for (auto w : state.black_markers)
        cerr << "Black Markers: " << w.first << " " << w.second << endl;

    cerr << endl
         << "State end" << endl;
}

string Board::check_for_consecutive_markers() {
    auto rings = state.get_player_rings(player);
    auto markers = state.get_player_markers(player);
    auto delete_from_set = [](decltype(state.black_markers) &s, const decltype(state.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    // auto contains_map = [](const decltype(state.board_map) &m, const decltype(state.board_map)::key_type k) { return m.find(k) != m.end(); };

    enum Directions { NE,
                      N,
                      NW,
                      SW,
                      S,
                      SE };

    const array<pair<Directions, Directions>, 3> directions = {make_pair(N, S), make_pair(NE, SW), make_pair(NW, SE)};

    auto sorted_coords_direction = [](State::Coordinate c, Directions d) -> vector<State::Coordinate> {
        vector<State::Coordinate> vec;
        auto predicate = [](int x, int y) {
            // Note the not
            return !((abs(x) == 5 && abs(y) == 5) || (abs(x) == 5 && abs(y) == 0) || (abs(x) == 0 && abs(y) == 5) || (abs(x - y) > 5) || (abs(x) > 5) || (abs(y) > 5));
        };

        if (d == NE) {
            for (int c1 = c.first + 1, c2 = c.second + 1; predicate(c1, c2); ++c1, ++c2)
                vec.emplace_back(c1, c2);
        } else if (d == N) {
            for (int c1 = c.first, c2 = c.second + 1; predicate(c1, c2); ++c2)
                vec.emplace_back(c1, c2);
        } else if (d == NW) {
            for (int c1 = c.first - 1, c2 = c.second; predicate(c1, c2); --c1)
                vec.emplace_back(c1, c2);
        } else if (d == SW) {
            for (int c1 = c.first - 1, c2 = c.second - 1; predicate(c1, c2); --c1, --c2)
                vec.emplace_back(c1, c2);
        } else if (d == S) {
            for (int c1 = c.first, c2 = c.second - 1; predicate(c1, c2); --c2)
                vec.emplace_back(c1, c2);
        } else if (d == SE) {
            for (int c1 = c.first + 1, c2 = c.second; predicate(c1, c2); ++c1)
                vec.emplace_back(c1, c2);
        }

        return vec;
    };

    auto adjacent_markers = [this](State::Coordinate c, vector<State::Coordinate> sorted_vec, Player p) -> vector<State::Coordinate> {
        auto marker = (p == WHITE) ? WHITE_MARKER : BLACK_MARKER;
        if (sorted_vec.size() > 0) {
            auto v = sorted_vec[0];
            if (!is_adjacent(v, c))
                throw std::invalid_argument("Non adjacent Markers");
        }
        decltype(sorted_vec) res;
        for (const auto &coord : sorted_vec) {
            if (state.board_map.at(coord) == marker) {
                res.push_back(coord);
                continue;
            }
            break;
        }

        return res;
    };

    vector<State::Coordinate> cons_markers;
    State::Coordinate start, end;

    bool found = false;
    for (const auto &m : markers) {
        for (const auto &d : directions) {
            auto adjacent_coords_1 = adjacent_markers(m, sorted_coords_direction(m, d.first), player);
            auto adjacent_coords_2 = adjacent_markers(m, sorted_coords_direction(m, d.second), player);

            if ((adjacent_coords_1.size() >= 2) && (adjacent_coords_2.size() >= 2)) {
                start = coord_to_hex(adjacent_coords_1[1]);
                end = coord_to_hex(adjacent_coords_2[1]);

                cons_markers.push_back(adjacent_coords_1[1]);
                cons_markers.push_back(adjacent_coords_1[0]);
                cons_markers.push_back(m);
                cons_markers.push_back(adjacent_coords_2[0]);
                cons_markers.push_back(adjacent_coords_2[1]);
                found = true;
                break;
            }
        }
    }

    string res;
    res.clear();

    if (!found)
        return res;

    for (const auto &m : cons_markers) {
        delete_from_set(markers, m);
        state.board_map[m] = EMPTY;
    }

    std::vector<State::Coordinate> coords;
    for (const auto &m : state.board_map)
        coords.push_back(m.first);

    long count = std::numeric_limits<long>::max();
    State::Coordinate ring;
    for (const auto &r : rings) {
        auto res = std::count_if(coords.begin(), coords.end(), [&r](const State::Coordinate &x) {
            return r.first == x.first || r.second == x.second || (r.second - r.first) == (x.second - x.first);
        });
        if (res < count) {
            count = res;
            ring = r;
        }
    }
    delete_from_set(rings, ring);
    state.board_map[ring] = EMPTY;

    ring = coord_to_hex(ring);
    res = "RS " + to_string(start.first) + " " + to_string(start.second) + " " +
          "RE" + " " + to_string(end.first) + " " + to_string(end.second) + " " + "X" +
          " " + to_string(ring.first) + " " + to_string(ring.second);

    return res;
}

string Board::remove_other_markers(string input) {
    istringstream ss(input);
    vector<string> vec;
    copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(vec));
    auto markers = state.get_other_player_markers(player);
    auto rings = state.get_other_player_rings(player);

    if (vec[0] != "RS")
        return input;

    auto it = find(vec.begin(), vec.end(), string("S"));
    if (it == vec.end())
        return input;

    vector<string> return_vector(it, vec.end());
    auto returnvalue = boost::algorithm::join(return_vector, " ");

    decltype(vec) strings_vec(vec.begin(), it);
    auto delete_from_set = [](decltype(state.black_markers) &s, const decltype(state.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };

    size_t k = 0;
    while (k < strings_vec.size()) {
        pair<int, int> p4 = hex_to_coord(make_pair(stoi(strings_vec[k + 1]), stoi(strings_vec[k + 2])));
        pair<int, int> p5 = hex_to_coord(make_pair(stoi(strings_vec[k + 4]), stoi(strings_vec[k + 5])));
        pair<int, int> p6 = hex_to_coord(make_pair(stoi(strings_vec[k + 7]), stoi(strings_vec[k + 8])));
        if (p4.first == p5.first) {
            if (p5.second > p4.second) {
                for (int i = p4.second; i <= p5.second; i++) {
                    state.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            } else {
                for (int i = p4.second; i >= p5.second; i--) {
                    state.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            }
        } else if (p4.second == p5.second) {
            if (p5.first > p4.first) {
                for (int i = p4.first; i <= p5.first; i++) {
                    state.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            } else {
                for (int i = p4.first; i >= p5.first; i--) {
                    state.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            }
        } else {
            if (p5.first > p4.first) {
                for (int i = p4.first, j = p4.second; i <= p5.first; i++, j++) {
                    state.board_map[make_pair(i, j)] = EMPTY;
                    delete_from_set(markers, make_pair(i, j));
                }
            } else {
                for (int i = p4.first, j = p4.second; i >= p5.first; i--, j--) {
                    state.board_map[make_pair(i, j)] = EMPTY;
                    delete_from_set(markers, make_pair(i, j));
                }
            }
        }
        state.board_map[p6] = EMPTY;
        delete_from_set(rings, p6);
        k = k + 9;
    }

    return returnvalue;
}