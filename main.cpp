#include <iostream>
#include "minimaxcheckers.h"

using namespace std;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4', 2, 1);
    // mmc.runAllTests();
    pair<string, int> startState;
    int depth = 10;

    startState.first = "02020202"
                       "20202020"
                       "00000000"
                       "00000000"
                       "00000000"
                       "00000000"
                       "01010101"
                       "10101010";

    mmc.printState(startState.first);
    cout << endl;

    for(int i = 0; i < 62; i++)
    {
        cout << "TURN: " << i << endl;

        if(i % 2 == 0)
        {
            cout << "1's turn:" << endl;
            startState = mmc.search(startState.first, depth, LONG_MIN, LONG_MAX, true);   
        }
        else
        {
            startState = mmc.search(startState.first, depth, LONG_MIN, LONG_MAX, false);   
            cout << "2's turn:" << endl;
        }

        cout << "Best move: " << endl;
        mmc.printState(startState.first);
        cout << "Static evaluation: " << startState.second << endl << endl;        
    }
    return 0;
}