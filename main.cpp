#include <iostream>
#include "minimaxcheckers.h"

using namespace std;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4');
    // mmc.runAllTests();
    pair<string, int> startState;

    startState.first = "02020202"
                       "20202020"
                       "00000000"
                       "00000000"
                       "00000000"
                       "00000000"
                       "01010101"
                       "10101010";

    for(int i = 0; i < 40; i++)
    {
        cout << "TURN: " << i << endl;

        if(i % 2 == 0)
        {
            cout << "1's turn:" << endl;
            startState = mmc.search(startState.first, 10, LONG_MIN, LONG_MAX, true);   
        }
        else
        {
            startState = mmc.search(startState.first, 10, LONG_MIN, LONG_MAX, false);   
            cout << "2's turn:" << endl;
        }

        cout << "Best move: " << endl;
        mmc.printState(startState.first);
        cout << "Static evaluation: " << startState.second << endl << endl;        
    }

    return 0;
}