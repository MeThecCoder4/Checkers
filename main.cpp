#include <iostream>
#include "game.h"
#include "pawn.h"

using namespace std;
using namespace Checkers;

int main()
{
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