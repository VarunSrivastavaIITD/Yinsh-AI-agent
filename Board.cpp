#include "Board.h"
#include "State.h"
#include <vector>
#include <utility>
#include <limits>

using namespace std;

auto const Placing = 10;
auto const Number_of_rings = 5;

vector<Ply> generate_pmode_plies(const State &state, Player player)
{
    return vector<Ply>();
}

bool is_connected(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
}

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

State perform_ply(const State &state, const Player &player, const Ply &ply_toperform)
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
        }
    }
    return newstate;
}

vector<Ply> generate_plies(const State &state, Player player)
{
    vector<Ply> plies;
    if (state.mode == P)
    {
        generate_pmode_plies(state, player);
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
    }
    return plies;
}

Ply Board::bestply()
{
}

double evaluation(const State &state)
{
    return 0;
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
        for (const auto &ply : plies)
        {

            const auto nextstate = perform_ply(state, player, ply);
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
        for (const auto &ply : plies)
        {
            const auto nextstate = perform_ply(state, player, ply);
            returned_beta = min(returned_beta, alphabeta(nextstate, alpha, beta, depth - 1, WHITE));
            if (alpha >= returned_beta)
                return returned_beta;
            beta = min(beta, returned_beta);
        }
        return_value = returned_beta;
    }

    return return_value;
}

State input_parse(const State &state, string s, const Player &player)
{
    State newstate = state;
    if(s[0]=='P')
    {
        pair <int,int> p1= make_pair((int)s[1],(int)s[2]);
        if(player ==  WHITE)
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
    else if(s[0]=='S')
    {
        pair <int,int> p2= make_pair((int)s[1],(int)s[2]);
        pair <int,int> p3= make_pair((int)s[4],(int)s[5]);
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
        if(search != rings.end())
            rings.erase(search);
        
        rings.insert(p3);        //add p3 to rings
        markers.insert(p2);     //add p2 to markers
        //toggle & change white&blackmarker, board_map
        if(p2.first == p3.first)
        {
            if(p3.second>p2.second)
            {
                for(int i=p2.second; i<=p3.second; i++)
                {
                    if(newstate.board_map[make_pair(p2.first,i)] == marker)
                    {
                        auto search = markers.find(make_pair(p2.first,i));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                        othermarkers.insert(make_pair(p2.first,i));
                    }
                    else if(newstate.board_map[make_pair(p2.first,i)] == othermarker)
                    {
                        auto search = othermarkers.find(make_pair(p2.first,i));
                        if(search != othermarkers.end())
                            othermarkers.erase(search);//remove from markers
                        markers.insert(make_pair(p2.first,i));
                    }
                }
            }
            else
            {
                for(int i=p2.second; i>=p3.second; i--)
                {
                    if(newstate.board_map[make_pair(p2.first,i)] == marker)
                    {
                        auto search = markers.find(make_pair(p2.first,i));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                        othermarkers.insert(make_pair(p2.first,i));
                    }
                    else if(newstate.board_map[make_pair(p2.first,i)] == othermarker)
                    {
                        auto search = othermarkers.find(make_pair(p2.first,i));
                        if(search != othermarkers.end())
                            othermarkers.erase(search);//remove from markers
                        markers.insert(make_pair(p2.first,i));
                    }
                }
            }
        }
        else if(p2.second == p3.second)
        {
            if(p3.first>p2.first)
            {
                for(int i=p2.first; i<=p3.first; i++)
                {
                    if(newstate.board_map[make_pair(p2.first,i)] == marker)
                    {
                        auto search = markers.find(make_pair(p2.first,i));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                        othermarkers.insert(make_pair(p2.first,i));
                    }
                    else if(newstate.board_map[make_pair(p2.first,i)] == othermarker)
                    {
                        auto search = othermarkers.find(make_pair(p2.first,i));
                        if(search != othermarkers.end())
                            othermarkers.erase(search);//remove from markers
                        markers.insert(make_pair(p2.first,i));
                    }
                }
            }
            else
            {
                for(int i=p2.first; i>=p3.first; i--)
                {
                    newstate.board_map[make_pair(i,p2.second)] = EMPTY;
                    auto search = markers.find(make_pair(i,p2.second));
                    if(search != markers.end())
                        markers.erase(search);//remove from markers
                }
            }
        }
        else
        {
            if(p3.first>p2.first)
            {
                for(int i=p2.first, j=p2.second; i<=p3.first; i++,j++)
                {
                    newstate.board_map[make_pair(i,j)] = EMPTY;
                    auto search = markers.find(make_pair(i,j));
                    if(search != markers.end())
                        markers.erase(search);//remove from markers
                }
            }
            else
            {
                for(int i=p2.first, j=p2.second; i>=p3.first; i--,j--)
                {
                    newstate.board_map[make_pair(i,j)] = EMPTY;
                    auto search = markers.find(make_pair(i,j));
                    if(search != markers.end())
                        markers.erase(search);//remove from markers
                }
            }
        }
        if(s.length()>6)
        {
            pair <int,int> p4= make_pair((int)s[8],(int)s[9]);
            pair <int,int> p5= make_pair((int)s[12],(int)s[13]);
            pair <int,int> p6= make_pair((int)s[15],(int)s[16]);
            decltype(newstate.white_rings) rings;
            decltype(newstate.white_markers) markers;
            if(player == WHITE)
            {
                rings = newstate.white_rings;
                markers = newstate.white_markers;
            }
            else
            {
                rings = newstate.black_rings;
                markers = newstate.black_markers;
            }
            if(p4.first == p5.first)
            {
                if(p5.second>p4.second)
                {
                    for(int i=p4.second; i<=p5.second; i++)
                    {
                        newstate.board_map[make_pair(p4.first,i)] = EMPTY;
                        auto search = markers.find(make_pair(p4.first,i));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
                else
                {
                    for(int i=p4.second; i>=p5.second; i--)
                    {
                        newstate.board_map[make_pair(p4.first,i)] = EMPTY;
                        auto search = markers.find(make_pair(p4.first,i));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
            }
            else if(p4.second == p5.second)
            {
                if(p5.first>p4.first)
                {
                    for(int i=p4.first; i<=p5.first; i++)
                    {
                        newstate.board_map[make_pair(i,p4.second)] = EMPTY;
                        auto search = markers.find(make_pair(i,p4.second));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
                else
                {
                    for(int i=p4.first; i>=p5.first; i--)
                    {
                        newstate.board_map[make_pair(i,p4.second)] = EMPTY;
                        auto search = markers.find(make_pair(i,p4.second));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
            }
            else
            {
                if(p5.first>p4.first)
                {
                    for(int i=p4.first, j=p4.second; i<=p5.first; i++,j++)
                    {
                        newstate.board_map[make_pair(i,j)] = EMPTY;
                        auto search = markers.find(make_pair(i,j));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
                else
                {
                    for(int i=p4.first, j=p4.second; i>=p5.first; i--,j--)
                    {
                        newstate.board_map[make_pair(i,j)] = EMPTY;
                        auto search = markers.find(make_pair(i,j));
                        if(search != markers.end())
                            markers.erase(search);//remove from markers
                    }
                }
            }
            newstate.board_map[p6]=EMPTY;
            auto search = rings.find(p6);
            if(search != rings.end())
                rings.erase(search);//remove from rings
        }
    }
    return newstate;
}

