#include <iostream>
using namespace std;
#include "Board.h"
#include "State.h"

int main() {
    cout << "hello";
    State s;
    s.board_map[make_pair(0, 0)] = WHITE_RING;
    Player p = BLACK;
    vector<Proper_Ply> plies = generate_plies(s, p);
    for (int i = 0; i < plies.size(); i++) {
        cout << "hey";
        cout << get<0>(plies[i]).first << " " << get<0>(plies[i]).second << endl;
        cout << get<1>(plies[i]).first << " " << get<1>(plies[i]).second << endl;
    }
}