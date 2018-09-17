#include "Board.h"
#include "State.h"
#include <limits>
#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

auto const Placing = 10;
auto const Number_of_rings = 5;
auto const K = 5;

double evaluation(const State &state) {
    return 0;
}

Ply Board::bestply() {
}

bool is_connected(pair<int, int> p1, pair<int, int> p2) {
    return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
}

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(const State &state, Player player, vector<Ply> &p);
State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform);
vector<Proper_Ply> generate_pmode_plies(const State &state);
bool add_ply(decltype(State::board_map) &bmap, const pair<int, int> p, vector<Ply> &vec, const pair<int, int> coordinate) {

    switch (bmap[p]) {
    case EMPTY:
        vec.emplace_back(coordinate, p);
        break;
    case WHITE_RING:
    case BLACK_RING:
        return true;
    case WHITE_MARKER:
    case BLACK_MARKER:
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
            for (int i = p2.second+1; i <= p3.second; i++) {
                if (newstate.board_map[make_pair(p2.first, i)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(p2.first, i));
                } else if (newstate.board_map[make_pair(p2.first, i)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(p2.first, i));
                }
            }
        }

        else {
            for (int i = p2.second-1; i >= p3.second; i--) {
                if (newstate.board_map[make_pair(p2.first, i)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(p2.first, i));
                } else if (newstate.board_map[make_pair(p2.first, i)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(p2.first, i));
                }
            }
        }
    }

    else if (p2.second == p3.second) {
        if (p3.first > p2.first) {
            for (int i = p2.first+1; i <= p3.first; i++) {
                if (newstate.board_map[make_pair(i, p2.second)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, p2.second));
                } else if (newstate.board_map[make_pair(i, p2.second)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, p2.second));
                }
            }
        } else {
            for (int i = p2.first-1; i >= p3.first; i--) {
                if (newstate.board_map[make_pair(i, p2.second)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, p2.second));
                } else if (newstate.board_map[make_pair(i, p2.second)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, p2.second));
                }
            }
        }
    } else {
        if (p3.first > p2.first) {
            for (int i = p2.first+1, j = p2.second+1; i <= p3.first; i++, j++) {
                if (newstate.board_map[make_pair(i, j)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, j));
                } else if (newstate.board_map[make_pair(i, j)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, j));
                }
            }
        } else {
            for (int i = p2.first-1, j = p2.second-1; i >= p3.first; i--, j--) {
                if (newstate.board_map[make_pair(i, j)] == WHITE_MARKER) {
                    flip(newstate.white_markers, newstate.black_markers, make_pair(i, j));
                } else if (newstate.board_map[make_pair(i, j)] == BLACK_MARKER) {
                    flip(newstate.black_markers, newstate.white_markers, make_pair(i, j));
                }
            }
        }
    }
    return newstate;
}

/*State remove_five_and_a_ring(const State &state, const Player &player)
{
    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    decltype(newstate.board_map)::mapped_type ring;
    decltype(newstate.board_map)::mapped_type marker;
    decltype(newstate.board_map)::mapped_type othermarker;
    decltype(newstate.white_rings) rings;
    decltype(newstate.white_markers) markers;
    decltype(newstate.white_markers) othermarkers;

    if (player == WHITE)
    {
        marker = WHITE_MARKER;
        othermarker = BLACK_MARKER;
        ring = WHITE_RING;
        markers = newstate.white_markers;
        othermarkers = newstate.black_markers;
        rings = newstate.white_rings;
    }
    else
    {
        marker = BLACK_MARKER;
        othermarker = WHITE_MARKER;
        ring = BLACK_RING;
        markers = newstate.black_markers;
        othermarkers = newstate.white_markers;
        rings = newstate.black_rings;
    }
    map<pair<int, int>, tuple<int, int, int>> marker_lines;
    for (auto i = markers.begin(); i != markers.end(); i++)
    {
        marker_lines.insert(*i, make_tuple(1, 1, 1));
    }
    auto bmap = newstate.board_map;
   
    for (const auto &coordinate : markers)
    {
        int markers_left;
        int markers_right;
        pair<int, int> left_most;
        if(get<0>(marker_lines[coordinate])==1)
        {
            markers_left = 0;
            markers_right = 0;
            for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y)
            {
                if (bmap[make_pair(coordinate.first, y)] != marker)
                    break;
                else
                {
                    markers_right++;
                    get<0>(marker_lines[make_pair(coordinate.first, y)]) = 0;
                }
            }

            for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y)
            {
                if (bmap[make_pair(coordinate.first, y)] != marker)
                    break;
                else
                {
                    markers_left++;
                    left_most = make_pair(coordinate.first, y);
                    get<0>(marker_lines[make_pair(coordinate.first, y)]) = 0;
                }
            }
            get<0>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
            if (!(markers_left + markers_right + 1 < 5))
            {
                auto m = left_most;
                for (int k = 0; k < K; k++, m.second++) //modify to erase all possible lines 
                {
                    newstate.board_map[m]==EMPTY;
                    delete_from_set(markers, m);
                }
                rings.erase(rings.begin()); //modify to erase all possible rings
            }
        }
        else if(get<1>(marker_lines[coordinate])==1)
        {
            markers_left = 0;
            markers_right = 0;
            for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x)
            {
                if (bmap[make_pair(x, coordinate.second)] != marker)
                    break;
                else
                {
                    markers_right++;
                    get<1>(marker_lines[make_pair(x, coordinate.second)]) = 0;
                }
            }

            for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x)
            {
                if (bmap[make_pair(x, coordinate.second)] != marker)
                    break;
                else
                {
                    markers_left++;
                    left_most = make_pair(x, coordinate.second);
                    get<1>(marker_lines[make_pair(x, coordinate.second)]) = 0;
                }
            }
            get<1>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
            if (!(markers_left + markers_right + 1 < 5))
            {
                auto m = left_most;
                for (int k = 0; k < K; k++, m.first++) //modify to erase all possible lines 
                {
                    newstate.board_map[m]==EMPTY;
                    delete_from_set(markers, m);
                }
                rings.erase(rings.begin()); //modify to erase all possible rings
            }
        }
        else if(get<2>(marker_lines[coordinate])==1)
        {
            markers_left = 0;
            markers_right = 0;
            for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y)
            {
                if (bmap[make_pair(x, coordinate.second)] != marker)
                    break;
                else
                {
                    markers_right++;
                    get<2>(marker_lines[make_pair(x, y)]) = 0;
                }
            }

            for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y)
            {
                if (bmap[make_pair(x, y)] != marker)
                    break;
                else
                {
                    markers_left++;
                    left_most = make_pair(x, y);
                    get<2>(marker_lines[make_pair(x, y)]) = 0;
                }
            }
            get<2>(marker_lines[make_pair(coordinate.first, coordinate.second)]) = 0;
            if (!(markers_left + markers_right + 1 < 5))
            {
                auto m = left_most;
                for (int k = 0; k < K; k++, m.first++,m.second++) //modify to erase all possible lines 
                {
                    newstate.board_map[m]==EMPTY;
                    delete_from_set(markers, m);
                }
                rings.erase(rings.begin()); //modify to erase all possible rings
            }
        }
    }
}*/

/*State perform_ply(const State &state, const Player &player, const Ply &ply_toperform)
{
    State newstate = state;
    if (ply_toperform.second.first == Placing && ply_toperform.second.second == Placing)
    {
        if (player == WHITE)
        {
            newstate.board_map[ply_toperform.first] = WHITE_RING;
            newstate.white_rings.insert(ply_toperform.first);
            if (newstate.white_rings.size() == Number_of_rings && newstate.black_rings.size() == Number_of_rings)
            {
                newstate.mode = S;
            }
        }
        else
        {
            newstate.board_map[ply_toperform.first] = BLACK_RING;
            newstate.black_rings.insert(ply_toperform.first);
            if (newstate.white_rings.size() == Number_of_rings && newstate.black_rings.size() == Number_of_rings)
            {
                newstate.mode = S;
            }
        }
    }
    else
    {
        if (player == WHITE)
        {
            newstate.board_map[ply_toperform.first] = WHITE_MARKER;
            newstate.board_map[ply_toperform.second] = WHITE_RING;
            auto search = newstate.white_rings.find(ply_toperform.first);
            if (search != newstate.white_rings.end())
            {
                newstate.white_rings.erase(search);
                newstate.white_rings.insert(ply_toperform.second);
            }
            newstate.white_markers.insert(ply_toperform.first);
        }
        else
        {
            newstate.board_map[ply_toperform.first] = BLACK_MARKER;
            newstate.board_map[ply_toperform.second] = BLACK_RING;
            auto search = newstate.black_rings.find(ply_toperform.first);
            if (search != newstate.black_rings.end())
            {
                newstate.black_rings.erase(search);
                newstate.black_rings.insert(ply_toperform.second);
            }
            newstate.black_markers.insert(ply_toperform.first);
            newstate = Toggle(ply_toperform.first, ply_toperform.second, newstate, player);
        }
    }
    return newstate;
}*/

vector<Proper_Ply> generate_plies(const State &state, Player player) {
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
                
        for (const auto &coordinate : rings)
        {
            for (auto y = coordinate.second+1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y)
            {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate))
                    break;
            }

            for (auto y = coordinate.second-1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y)
            {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first+1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x)
            {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first-1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x)
            {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first+1, y = coordinate.second+1; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y)
            {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first-1, y = coordinate.second-1; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y)
            {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate))
                    break;
            }
        }
        cout<<plies.size()<<"##";
        proper_plies = Generating_proper_moves_from_selection_moves(state, player, plies);
        /*for (auto q = plies.begin(); q != plies.end(); q++)
        {
            cout<<(*q).first.first<<" "<<(*q).first.second<<endl;
            cout<<(*q).second.first<<" "<<(*q).second.second<<endl;
            cout<<endl;
            proper_plies = Generating_proper_moves_from_selection_moves(state, *q, player, proper_plies);
        }*/
    }
    return proper_plies;
}

double alphabeta(const State &state, double alpha, double beta, int depth, Player player) {
    if (depth == 0) {
        return evaluation(state);
    }

    auto plies = generate_plies(state, player);
    auto return_value = 0.0;

    if (player == WHITE) {
        auto returned_alpha = numeric_limits<double>::min();
        for (const auto &proper_ply : plies) {

            const auto nextstate = perform_proper_ply(state, player, proper_ply);
            returned_alpha = max(returned_alpha, alphabeta(nextstate, alpha, beta, depth - 1, BLACK));
            if (returned_alpha >= beta)
                return returned_alpha;
            alpha = max(alpha, returned_alpha);
        }
        return_value = returned_alpha;
    }

    else {
        auto returned_beta = numeric_limits<double>::max();
        for (const auto &proper_ply : plies) {
            const auto nextstate = perform_proper_ply(state, player, proper_ply);
            returned_beta = min(returned_beta, alphabeta(nextstate, alpha, beta, depth - 1, WHITE));
            if (alpha >= returned_beta)
                return returned_beta;
            beta = min(beta, returned_beta);
        }
        return_value = returned_beta;
    }

    return return_value;
}

pair<int, int> convert_to_our(pair<int, int> p) {
    return p;
}

State Board::input_parse(string s, const State &state, const Player &player) {

    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    if (s[0] == 'P') {
        pair<int, int> p1 = convert_to_our(make_pair((s[8]-'2'), (s[4]-'0')));
        if (player == WHITE) {
            newstate.board_map[p1] = WHITE_RING;
            newstate.white_rings.insert(p1);
        } else {
            newstate.board_map[p1] = BLACK_RING;
            newstate.black_rings.insert(p1);
        }
    } else if (s[0] == 'S') {
        //cout<<(s[8]-'0')<<"**"<<endl;
        pair<int, int> p2 = convert_to_our(make_pair((s[2]-'0'), (s[4]-'0')));
        pair<int, int> p3 = convert_to_our(make_pair((s[8]-'0'), (s[10]-'0')));
        decltype(newstate.board_map)::mapped_type ring;
        decltype(newstate.board_map)::mapped_type marker;
        decltype(newstate.white_rings) &rings = newstate.white_rings;
        decltype(newstate.white_markers) &markers = newstate.white_markers;
        decltype(newstate.white_markers) &othermarkers = newstate.black_markers;

        if (player == WHITE) {
            marker = WHITE_MARKER;
            ring = WHITE_RING;
            markers = newstate.white_markers;
            othermarkers = newstate.black_markers;
            rings = newstate.white_rings;
        } else {
            marker = BLACK_MARKER;
            ring = BLACK_RING;
            markers = newstate.black_markers;
            othermarkers = newstate.white_markers;
            rings = newstate.black_rings;
        }
        newstate.board_map[p2] = marker;
        newstate.board_map[p3] = ring;

        auto search = rings.find(p2);
        if (search != rings.end())
            rings.erase(search);

        //cout<<"&&"<<p3.first<<" "<<p3.second<<endl;
        //cout<<"&&"<<newstate.board_map[make_pair(3,  2)]<<endl;
        rings.insert(p3);   //add p3 to rings
        markers.insert(p2); //add p2 to markers
        //toggle & change white&blackmarker, board_map
        newstate = Toggle(p2, p3, newstate);

        if (s.length() > 11) {
            int k = 13;
            while (s[k] != '\0') {
                pair<int, int> p4 = convert_to_our(make_pair((s[k+2]-'0'), (s[k+4]-'0')));
                pair<int, int> p5 = convert_to_our(make_pair((s[k+9]-'0'), (s[k+11]-'0')));
                pair<int, int> p6 = convert_to_our(make_pair((s[k+16]-'0'), (s[k+18]-'0')));
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
                k = k + 21;
            }
        }
    }
    //cout<<"&&"<<newstate.board_map[make_pair(5,  4)]<<endl;
    //cout<<"&&"<<newstate.board_map[make_pair(3,  2)]<<endl;
    return newstate;
}

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(const State &state, Player player, vector<Ply> &plies) {
    
    vector<Proper_Ply> proper_p;
    for (auto q = plies.begin(); q != plies.end(); q++)
    {
        State newstate = state;
        //cout<<(*q).first.first<<" "<<(*q).first.second<<endl;
        //cout<<(*q).second.first<<" "<<(*q).second.second<<endl;
        //cout<<endl;
        //proper_plies = Generating_proper_moves_from_selection_moves(state, *q, player, proper_plies);
        Ply p=*q;
        decltype(state.board_map)::mapped_type ring;
        decltype(state.board_map)::mapped_type marker;
        decltype(state.white_rings) &rings = newstate.white_rings;
        decltype(state.white_rings) &otherrings = newstate.black_rings;
        decltype(state.white_markers) &markers = newstate.white_markers;
        decltype(state.white_markers) &othermarkers = newstate.black_markers;

        if (player == WHITE) {
            marker = WHITE_MARKER;
            ring = WHITE_RING;
            markers = newstate.white_markers;
            othermarkers = newstate.black_markers;
            rings = newstate.white_rings;
            otherrings = newstate.black_rings;
        } else {
            marker = BLACK_MARKER;
            ring = BLACK_RING;
            markers = newstate.black_markers;
            othermarkers = newstate.white_markers;
            rings = newstate.black_rings;
            otherrings = newstate.white_rings;
        }
        // implementing ply before converting to properply
        newstate.board_map[p.first] = marker;
        newstate.board_map[p.second] = ring;
        auto search = rings.find(p.first);
        if (search != rings.end()) {
            rings.erase(search);
            rings.insert(p.second);
        }
        markers.insert(p.first);
        newstate = Toggle(p.first, p.second, newstate);

        map<pair<int, int>, tuple<int, int, int>> marker_lines;
        for (auto i = markers.begin(); i != markers.end(); i++) {
            marker_lines[*i] = make_tuple(1, 1, 1);
        }
        auto bmap = newstate.board_map;
        int markers_left;
        int markers_right;
        pair<int, int> left_most;
        int flag=0;
        for (const auto &coordinate : markers) {
            if (get<0>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto y = coordinate.second+1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y) {
                    if (bmap[make_pair(coordinate.first, y)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<0>(marker_lines[make_pair(coordinate.first, y)]) = 0;
                    }
                }
                //cout<<markers_right<<"R ";
                for (auto y = coordinate.second-1; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y) {
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
                        for (const auto &coordinate2 : rings) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first, left_most.second + d), make_pair(left_most.first, left_most.second + 4 + d), coordinate2);
                        }
                    }
                } 
                else
                {
                    //cout<<"inelse";
                    flag+=1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            if (get<1>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto x = coordinate.first+1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x) {
                    if (bmap[make_pair(x, coordinate.second)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<1>(marker_lines[make_pair(x, coordinate.second)]) = 0;
                    }
                }

                for (auto x = coordinate.first-1; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x) {
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
                        for (const auto &coordinate2 : rings) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first + d, left_most.second), make_pair(left_most.first + 4 + d, left_most.second), coordinate2);
                        }
                    }
                } 
                else
                {
                    //cout<<"inelse";
                    flag+=1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            if (get<2>(marker_lines[coordinate]) == 1) {
                markers_left = 0;
                markers_right = 0;
                for (auto x = coordinate.first+1, y = coordinate.second+1; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y) {
                    if (bmap[make_pair(x, coordinate.second)] != marker)
                        break;
                    else {
                        markers_right++;
                        get<2>(marker_lines[make_pair(x, y)]) = 0;
                    }
                }

                for (auto x = coordinate.first-1, y = coordinate.second-1; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y) {
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
                        for (const auto &coordinate2 : rings) {
                            proper_p.emplace_back(p.first, p.second, make_pair(left_most.first + d, left_most.second + d), make_pair(left_most.first + 4 + d, left_most.second + 4 + d), coordinate2);
                        }
                    }
                } 
                else
                {
                    //cout<<"inelse";
                    flag+=1;
                    //proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
                }
            }
            //cout<<flag<<"$$";
            if(flag==3)
            {
                cout<<"yay";
                proper_p.emplace_back(p.first, p.second, make_pair(10, 10), make_pair(10, 10), make_pair(10, 10));
            }
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
    decltype(state.white_rings) &rings = newstate.white_rings;
    decltype(state.white_rings) &otherrings = newstate.black_rings;
    decltype(state.white_markers) &markers = newstate.white_markers;
    decltype(state.white_markers) &othermarkers = newstate.black_markers;

    if (player == WHITE) {
        marker = WHITE_MARKER;
        ring = WHITE_RING;
        markers = newstate.white_markers;
        othermarkers = newstate.black_markers;
        rings = newstate.white_rings;
        otherrings = newstate.black_rings;
    } else {
        marker = BLACK_MARKER;
        ring = BLACK_RING;
        markers = newstate.black_markers;
        othermarkers = newstate.white_markers;
        rings = newstate.black_rings;
        otherrings = newstate.white_rings;
    }
    if (get<1>(proper_ply_toperform) == make_pair(10, 10)) {
        newstate.board_map[get<0>(proper_ply_toperform)] = ring;
        rings.insert(get<0>(proper_ply_toperform));
        if (rings.size() == Number_of_rings && otherrings.size() == Number_of_rings) {
            newstate.mode = S;
        }
    } else if (get<2>(proper_ply_toperform) == make_pair(10, 10)) {
        cout<<"incorrect";
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.find(get<0>(proper_ply_toperform));
        if (search != rings.end()) {
            rings.erase(search);
            rings.insert(get<1>(proper_ply_toperform));
        }
        markers.insert(get<0>(proper_ply_toperform));
        newstate = Toggle(get<0>(proper_ply_toperform), get<1>(proper_ply_toperform), newstate);
    } else {
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.find(get<0>(proper_ply_toperform));
        if (search != rings.end()) {
            rings.erase(search);
            rings.insert(get<1>(proper_ply_toperform));
        }
        markers.insert(get<0>(proper_ply_toperform));
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

void Board::printboard() {
}