#include <iostream>
#include "game.h"

using namespace std;
using namespace Checkers;

int main()
{
    try
    {
        Game game(8, true);
    }
    catch(const char* e)
    {
        cout << e << endl;
    }

    return 0;
}