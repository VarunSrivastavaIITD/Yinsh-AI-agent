#include "Board.h"
#include "State.h"
#include <vector>
#include <utility>

using namespace std;

enum Piece
{
    EMPTY_PIECE,
    RING,
    MARKER
};

vector<Ply> generate_pmode_plies(const State &state, Player player)
{
    return vector<Ply>();
}

bool is_connected(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first == p2.first || p1.second == p2.second || (p1.second - p1.first) == (p2.second - p2.first);
}

Piece is_piece_allowed(map<pair<int, int>, Values> bmap, pair<int, int> p)
{

    switch (bmap[p])
    {
    case WHITE_RING:
    case BLACK_RING:
        return RING;
    case WHITE_MARKER:
    case BLACK_MARKER:
        return MARKER;
    }

    return EMPTY_PIECE;
}

vector<Ply> Board::generate_plies(const State &state, Player player)
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
                auto piece = is_piece_allowed(bmap, make_pair(coordinate.first, y));
            }

            for (auto x = coordinate.first; bmap.find(make_pair(x, coordinate.second)) != bmap.end(); ++x)
            {
            }

            for (auto x = coordinate.first, y = coordinate.second; bmap.find(make_pair(x, y)) != bmap.end(); ++x, ++y)
            {
            }
        }
    }
    return vector<Ply>();
}
