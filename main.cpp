#include <iostream>
#include <vector>
#include "State.h"

using namespace std;

int main()
{
    vector<State> arr(2000000);
    std::cout << (float)(arr.size() * sizeof(arr[0])) / (1024.0 * 1024.0) << std::endl;
    return 0;
}