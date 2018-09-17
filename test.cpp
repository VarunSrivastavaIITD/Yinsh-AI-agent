#include <iostream>
using namespace std;
#include "Board.h"
#include "State.h"

int main() {
    //cout << "hello"<<endl;
    Board b;
    b.state.board_map[make_pair(0, 0)] = WHITE_RING;
    b.state.board_map[make_pair(-1, 0)] = BLACK_RING;
    b.state.board_map[make_pair(1, 1)] = WHITE_RING;
    b.state.board_map[make_pair(0, -1)] = BLACK_RING;
    b.state.board_map[make_pair(1, -1)] = WHITE_RING;
    b.state.board_map[make_pair(0, 1)] = BLACK_RING;
    b.state.board_map[make_pair(3, 2)] = WHITE_RING;
    b.state.board_map[make_pair(1, 0)] = BLACK_RING;
    b.state.board_map[make_pair(2, -1)] = WHITE_RING;
    b.state.board_map[make_pair(-1, 1)] = BLACK_RING;
    b.state.board_map[make_pair(1, 2)] = WHITE_MARKER;
    b.state.board_map[make_pair(2, 2)] = WHITE_MARKER;
    b.state.board_map[make_pair(3, 2)] = WHITE_MARKER;
    b.state.board_map[make_pair(4, 2)] = WHITE_MARKER;
    b.state.board_map[make_pair(5, 2)] = WHITE_MARKER;
    b.state.white_rings.insert(make_pair(0, 0));
    b.state.black_rings.insert(make_pair(-1, 0));
    b.state.white_rings.insert(make_pair(1, 1));
    b.state.black_rings.insert(make_pair(0, -1));
    b.state.white_rings.insert(make_pair(1, -1));
    b.state.black_rings.insert(make_pair(0, 1));
    b.state.white_rings.insert(make_pair(3, 2));
    b.state.black_rings.insert(make_pair(1, 0));
    b.state.white_rings.insert(make_pair(2, -1));
    b.state.black_rings.insert(make_pair(-1, 1));
    b.state.white_markers.insert(make_pair(1, 2));
    b.state.white_markers.insert(make_pair(2, 2));
    b.state.white_markers.insert(make_pair(3, 2));
    b.state.white_markers.insert(make_pair(4, 2));
    b.state.white_markers.insert(make_pair(5, 2));
    b.state.mode = S;
    b.player = WHITE;
    string str = "S 3 2 M 2 2";
    //b.state = b.input_parse(str, b.state, b.player);
    //cout<<b.state.board_map[make_pair(1,  2)]<<"$$$$$"<<endl;
    //cout<<b.state.board_map[make_pair(4,  2)]<<"$$$$$"<<endl;
    //cout<<"@@"<<atoi(str[8])<<endl;
    Proper_Ply p2;
    get<0>(p2) = make_pair(2, 2);
    get<1>(p2) = make_pair(4, 4);
    get<2>(p2) = make_pair(10, 10);
    get<3>(p2) = make_pair(10, 10);
    get<4>(p2) = make_pair(10, 10);
    State c = checkfor5(b.state, b.player);
    //cout<<c.board_map[make_pair(2,  2)]<<"$$$$$"<<endl;
    //cout<<c.board_map[make_pair(4,  4)]<<"$$$$$"<<endl;
    //cout<<c.board_map[make_pair(3,  2)]<<"$$$$$"<<endl;
    //cout<<c.board_map[make_pair(4,  2)]<<"$$$$$"<<endl;
    /*vector<Proper_Ply> plies = generate_plies(b.state, b.player);
    cout<<"**"<<plies.size()<<endl;
    for (int i = 0; i < plies.size(); i++) 
    {
        //cout << "hey";
        cout << get<0>(plies[i]).first << " " << get<0>(plies[i]).second << "   ";
        cout << get<1>(plies[i]).first << " " << get<1>(plies[i]).second << "   ";
        cout << get<2>(plies[i]).first << " " << get<2>(plies[i]).second << "   ";
        cout << get<3>(plies[i]).first << " " << get<3>(plies[i]).second << "   ";
        cout << get<4>(plies[i]).first << " " << get<4>(plies[i]).second << endl;
    }   */
}