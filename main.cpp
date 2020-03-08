#include <iostream>
#include "game.h"
#include "pawn.h"

using namespace std;
using namespace Checkers;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4');

    string state12 = "00000000"
                     "00000020"
                     "00000200"
                     "00201000"
                     "00000000"
                     "00100000"
                     "00000000"
                     "00000000";

    auto resp = mmc.search(state12, 7, LONG_MIN, LONG_MAX, true);
    mmc.printState(resp.first);
    cout << "Static: " << resp.second << endl;
    try
    {
        Game game(true);
    }
    catch(const char* e)
    {
        cout << e << endl;
    }

    return 0;
}