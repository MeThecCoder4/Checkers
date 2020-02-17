#include <iostream>
#include "minimaxcheckers.h"

using namespace std;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4');
    string startState = "02020202"
                        "20002020"
                        "00000000"
                        "00000000"
                        "00020000"
                        "00100000"
                        "01000101"
                        "10101010";

    pair<string, int> child = mmc.search(startState, 7, true);
    cout << "Best move: " << endl;
    mmc.printState(child.first);
    cout << "Static evaluation: " << child.second << endl;
    return 0;
}