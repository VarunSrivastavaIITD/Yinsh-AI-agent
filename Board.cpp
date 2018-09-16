#include "Board.h"
#include "State.h"
#include <vector>
#include <utility>
#include <limits>
#include <tuple>

using namespace std;

auto const Placing = 10;
auto const Number_of_rings = 5;
auto const K = 5;

double evaluation(const State &state)
{
    return 0;
}

Ply Board::bestply()
{
}

bool is_connected(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
}

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(State, Ply, Player);
State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform);
vector<Proper_Ply> generate_pmode_plies(State state, Player player);
bool add_ply(map<pair<int, int>, Values> &bmap, const pair<int, int> p, vector<Ply> &vec, const pair<int, int> coordinate)
{

    switch (bmap[p])
    {
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

State Toggle(pair<int, int> p2, pair<int, int> p3, State state, Player player)
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
    if (p2.first == p3.first)
    {
        if (p3.second > p2.second)
        {
            for (int i = p2.second; i <= p3.second; i++)
            {
                if (newstate.board_map[make_pair(p2.first, i)] == marker)
                {
                    delete_from_set(markers, make_pair(p2.first, i));
                    othermarkers.insert(make_pair(p2.first, i));
                }
                else if (newstate.board_map[make_pair(p2.first, i)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(p2.first, i));
                    markers.insert(make_pair(p2.first, i));
                }
            }
        }
        else
        {
            for (int i = p2.second; i >= p3.second; i--)
            {
                if (newstate.board_map[make_pair(p2.first, i)] == marker)
                {
                    delete_from_set(markers, make_pair(p2.first, i));
                    othermarkers.insert(make_pair(p2.first, i));
                }
                else if (newstate.board_map[make_pair(p2.first, i)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(p2.first, i));
                    markers.insert(make_pair(p2.first, i));
                }
            }
        }
    }
    else if (p2.second == p3.second)
    {
        if (p3.first > p2.first)
        {
            for (int i = p2.first; i <= p3.first; i++)
            {
                if (newstate.board_map[make_pair(i, p2.second)] == marker)
                {
                    delete_from_set(markers, make_pair(i, p2.second));
                    othermarkers.insert(make_pair(i, p2.second));
                }
                else if (newstate.board_map[make_pair(i, p2.second)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(i, p2.second));
                    markers.insert(make_pair(i, p2.second));
                }
            }
        }
        else
        {
            for (int i = p2.first; i >= p3.first; i--)
            {
                if (newstate.board_map[make_pair(i, p2.second)] == marker)
                {
                    delete_from_set(markers, make_pair(i, p2.second));
                    othermarkers.insert(make_pair(i, p2.second));
                }
                else if (newstate.board_map[make_pair(i, p2.second)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(i, p2.second));
                    markers.insert(make_pair(i, p2.second));
                }
            }
        }
    }
    else
    {
        if (p3.first > p2.first)
        {
            for (int i = p2.first, j = p2.second; i <= p3.first; i++, j++)
            {
                if (newstate.board_map[make_pair(i, j)] == marker)
                {
                    delete_from_set(markers, make_pair(i, j));
                    othermarkers.insert(make_pair(i, j));
                }
                else if (newstate.board_map[make_pair(i, j)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(i, j));
                    markers.insert(make_pair(i, j));
                }
            }
        }
        else
        {
            for (int i = p2.first, j = p2.second; i >= p3.first; i--, j--)
            {
                if (newstate.board_map[make_pair(i, j)] == marker)
                {
                    delete_from_set(markers, make_pair(i, j));
                    othermarkers.insert(make_pair(i, j));
                }
                else if (newstate.board_map[make_pair(i, j)] == othermarker)
                {
                    delete_from_set(othermarkers, make_pair(i, j));
                    markers.insert(make_pair(i, j));
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

vector<Proper_Ply> generate_plies(const State &state, Player player)
{
    vector<Proper_Ply> proper_plies;
    vector<Ply> plies;
    if (state.mode == P)
    {
        proper_plies = generate_pmode_plies(state, player);
    }
    else
    {
        auto bmap = state.board_map;
        decltype(state.white_rings) rings;

        if (player == WHITE)
            rings = state.white_rings;
        else
            rings = state.black_rings;

        for (const auto &coordinate : rings)
        {
            for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); ++y)
            {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate))
                    break;
            }

            for (auto y = coordinate.second; bmap.find(make_pair(coordinate.first, y)) != bmap.end(); --y)
            {
                if (add_ply(bmap, make_pair(coordinate.first, y), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x)
            {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); --x)
            {
                if (add_ply(bmap, make_pair(x, coordinate.second), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y)
            {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate))
                    break;
            }

            for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); --x, --y)
            {
                if (add_ply(bmap, make_pair(x, y), plies, coordinate))
                    break;
            }
        }
        for (auto q = plies.begin(); q!=plies.end(); q++)
        {
            proper_plies = Generating_proper_moves_from_selection_moves(state, *q, player);
        }
    }
    return proper_plies;
}

double alphabeta(const State &state, double alpha, double beta, int depth, Player player)
{
    if (depth == 0)
    {
        return evaluation(state);
    }

    auto plies = generate_plies(state, player);
    auto return_value = 0.0;

    if (player == WHITE)
    {
        auto returned_alpha = numeric_limits<double>::min();
        for (const auto &proper_ply : plies)
        {

            const auto nextstate = perform_proper_ply(state, player, proper_ply);
            returned_alpha = max(returned_alpha, alphabeta(nextstate, alpha, beta, depth - 1, BLACK));
            if (returned_alpha >= beta)
                return returned_alpha;
            alpha = max(alpha, returned_alpha);
        }
        return_value = returned_alpha;
    }

    else
    {
        auto returned_beta = numeric_limits<double>::max();
        for (const auto &proper_ply : plies)
        {
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

State Board::input_parse(string s)
{
    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    if (s[0] == 'P')
    {
        pair<int, int> p1 = make_pair((int)s[1], (int)s[2]);
        if (player == WHITE)
        {
            newstate.board_map[p1] = WHITE_RING;
            newstate.white_rings.insert(p1);
        }
        else
        {
            newstate.board_map[p1] = BLACK_RING;
            newstate.black_rings.insert(p1);
        }
    }
    else if (s[0] == 'S')
    {
        pair<int, int> p2 = make_pair((int)s[1], (int)s[2]);
        pair<int, int> p3 = make_pair((int)s[4], (int)s[5]);
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
        newstate.board_map[p2] = marker;
        newstate.board_map[p3] = ring;

        auto search = rings.find(p2);
        if (search != rings.end())
            rings.erase(search);

        rings.insert(p3);   //add p3 to rings
        markers.insert(p2); //add p2 to markers
        //toggle & change white&blackmarker, board_map
        newstate = Toggle(p2, p3, newstate, player);

        if (s.length() > 6)
        {
            pair<int, int> p4 = make_pair((int)s[8], (int)s[9]);
            pair<int, int> p5 = make_pair((int)s[12], (int)s[13]);
            pair<int, int> p6 = make_pair((int)s[15], (int)s[16]);
            decltype(newstate.white_rings) rings;
            decltype(newstate.white_markers) markers;
            if (player == WHITE)
            {
                rings = newstate.white_rings;
                markers = newstate.white_markers;
            }
            else
            {
                rings = newstate.black_rings;
                markers = newstate.black_markers;
            }
            if (p4.first == p5.first)
            {
                if (p5.second > p4.second)
                {
                    for (int i = p4.second; i <= p5.second; i++)
                    {
                        newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                        delete_from_set(markers, make_pair(p4.first, i));
                    }
                }
                else
                {
                    for (int i = p4.second; i >= p5.second; i--)
                    {
                        newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                        delete_from_set(markers, make_pair(p4.first, i));
                    }
                }
            }
            else if (p4.second == p5.second)
            {
                if (p5.first > p4.first)
                {
                    for (int i = p4.first; i <= p5.first; i++)
                    {
                        newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                        delete_from_set(markers, make_pair(i, p4.second));
                    }
                }
                else
                {
                    for (int i = p4.first; i >= p5.first; i--)
                    {
                        newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                        delete_from_set(markers, make_pair(i, p4.second));
                    }
                }
            }
            else
            {
                if (p5.first > p4.first)
                {
                    for (int i = p4.first, j = p4.second; i <= p5.first; i++, j++)
                    {
                        newstate.board_map[make_pair(i, j)] = EMPTY;
                        delete_from_set(markers, make_pair(i, j));
                    }
                }
                else
                {
                    for (int i = p4.first, j = p4.second; i >= p5.first; i--, j--)
                    {
                        newstate.board_map[make_pair(i, j)] = EMPTY;
                        delete_from_set(markers, make_pair(i, j));
                    }
                }
            }
            newstate.board_map[p6] = EMPTY;
            delete_from_set(rings, p6);
        }
    }
    return newstate;
}

vector<Proper_Ply> Generating_proper_moves_from_selection_moves(State state, Ply p, Player player)
{
    vector<Proper_Ply> proper_p;
    decltype(state.board_map)::mapped_type ring;
    decltype(state.board_map)::mapped_type marker;
    decltype(state.board_map)::mapped_type othermarker;
    decltype(state.white_rings) rings;
    decltype(state.white_markers) markers;
    decltype(state.white_markers) othermarkers;

    if (player == WHITE)
    {
        marker = WHITE_MARKER;
        othermarker = BLACK_MARKER;
        ring = WHITE_RING;
        markers = state.white_markers;
        othermarkers = state.black_markers;
        rings = state.white_rings;
    }
    else
    {
        marker = BLACK_MARKER;
        othermarker = WHITE_MARKER;
        ring = BLACK_RING;
        markers = state.black_markers;
        othermarkers = state.white_markers;
        rings = state.black_rings;
    }
    map<pair<int, int>, tuple<int, int, int>> marker_lines;
    for (auto i = markers.begin(); i != markers.end(); i++)
    {
        marker_lines.insert(*i, make_tuple(1, 1, 1));
    }
    auto bmap = state.board_map;
   
    for (const auto &coordinate : markers)
    {
        int markers_left;
        int markers_right;
        pair<int,int> left_most;
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
                for(int d=0;d<=(markers_left + markers_right + 1 - 5);d++)
                {
                    for (const auto &coordinate2 : rings)
                    {           
                        proper_p.emplace_back(p.first,p.second,make_pair(left_most.first,left_most.second+d),make_pair(left_most.first,left_most.second+4+d),coordinate2);
                    }
                }
            }
            else
                proper_p.emplace_back(p.first,p.second,make_pair(10,10),make_pair(10,10),make_pair(10,10));
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
                for(int d=0;d<=(markers_left + markers_right + 1 - 5);d++)
                {
                    for (const auto &coordinate2 : rings)
                    {           
                        proper_p.emplace_back(p.first,p.second,make_pair(left_most.first+d,left_most.second),make_pair(left_most.first+4+d,left_most.second),coordinate2);
                    }
                }
            }
            else
                proper_p.emplace_back(p.first,p.second,make_pair(10,10),make_pair(10,10),make_pair(10,10));
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
                for(int d=0;d<=(markers_left + markers_right + 1 - 5);d++)
                {
                    for (const auto &coordinate2 : rings)
                    {           
                        proper_p.emplace_back(p.first,p.second,make_pair(left_most.first+d,left_most.second+d),make_pair(left_most.first+4+d,left_most.second+4+d),coordinate2);
                    }
                }
            }
            else
                proper_p.emplace_back(p.first,p.second,make_pair(10,10),make_pair(10,10),make_pair(10,10));
        }
    }
}

vector<Proper_Ply> generate_pmode_plies(State state, Player player)
{
    vector<Proper_Ply> proper_p;
    for (auto it = state.board_map.begin(); it != state.board_map.end(); ++it) 
    {
        if(it->second == EMPTY)
        {
            proper_p.emplace_back(it->first,make_pair(10,10),make_pair(10,10),make_pair(10,10),make_pair(10,10));
        }
    }
}
State perform_proper_ply(const State &state, const Player &player, const Proper_Ply &proper_ply_toperform)
{
    State newstate = state;
    auto delete_from_set = [](decltype(newstate.black_markers) &s, const decltype(newstate.black_markers)::value_type &e) {
        auto search = s.find(e);
        if (search != s.end())
            s.erase(search);
    };
    vector<Proper_Ply> proper_p;
    decltype(state.board_map)::mapped_type ring;
    decltype(state.board_map)::mapped_type marker;
    decltype(state.board_map)::mapped_type othermarker;
    decltype(state.white_rings) rings;
    decltype(state.white_rings) otherrings;
    decltype(state.white_markers) markers;
    decltype(state.white_markers) othermarkers;

    if (player == WHITE)
    {
        marker = WHITE_MARKER;
        othermarker = BLACK_MARKER;
        ring = WHITE_RING;
        markers = newstate.white_markers;
        othermarkers = newstate.black_markers;
        rings = newstate.white_rings;
        otherrings = newstate.black_rings;
    }
    else
    {
        marker = BLACK_MARKER;
        othermarker = WHITE_MARKER;
        ring = BLACK_RING;
        markers = newstate.black_markers;
        othermarkers = newstate.white_markers;
        rings = newstate.black_rings;
        otherrings = newstate.white_rings;
    }
    if (get<1>(proper_ply_toperform) == make_pair(10,10))
    {
        newstate.board_map[get<0>(proper_ply_toperform)] = ring;
        rings.insert(get<0>(proper_ply_toperform));
        if (rings.size() == Number_of_rings && otherrings.size() == Number_of_rings)
        {
            newstate.mode = S;
        }
    }
    else if(get<2>(proper_ply_toperform) == make_pair(10,10))
    {
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.find(get<0>(proper_ply_toperform));
        if (search != rings.end())
        {
            rings.erase(search);
            rings.insert(get<1>(proper_ply_toperform));
        }
        markers.insert(get<0>(proper_ply_toperform));
        newstate = Toggle(get<0>(proper_ply_toperform), get<1>(proper_ply_toperform), newstate, player);
    }
    else
    {
        newstate.board_map[get<0>(proper_ply_toperform)] = marker;
        newstate.board_map[get<1>(proper_ply_toperform)] = ring;
        auto search = rings.find(get<0>(proper_ply_toperform));
        if (search != rings.end())
        {
            rings.erase(search);
            rings.insert(get<1>(proper_ply_toperform));
        }
        markers.insert(get<0>(proper_ply_toperform));
        newstate = Toggle(get<0>(proper_ply_toperform), get<1>(proper_ply_toperform), newstate, player);
        
        pair<int, int> p4 = get<2>(proper_ply_toperform);
        pair<int, int> p5 = get<3>(proper_ply_toperform);
        pair<int, int> p6 = get<4>(proper_ply_toperform);
        if (p4.first == p5.first)
        {
            if (p5.second > p4.second)
            {
                for (int i = p4.second; i <= p5.second; i++)
                {
                    newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            }
            else
            {
                for (int i = p4.second; i >= p5.second; i--)
                {
                    newstate.board_map[make_pair(p4.first, i)] = EMPTY;
                    delete_from_set(markers, make_pair(p4.first, i));
                }
            }
        }
        else if (p4.second == p5.second)
        {
            if (p5.first > p4.first)
            {
                for (int i = p4.first; i <= p5.first; i++)
                {
                    newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            }
            else
            {
                for (int i = p4.first; i >= p5.first; i--)
                {
                    newstate.board_map[make_pair(i, p4.second)] = EMPTY;
                    delete_from_set(markers, make_pair(i, p4.second));
                }
            }
        }
        else
        {
            if (p5.first > p4.first)
            {
                for (int i = p4.first, j = p4.second; i <= p5.first; i++, j++)
                {
                    newstate.board_map[make_pair(i, j)] = EMPTY;
                    delete_from_set(markers, make_pair(i, j));
                }
            }
            else
            {
                for (int i = p4.first, j = p4.second; i >= p5.first; i--, j--)
                {
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

void Board::printboard()
{
    
}