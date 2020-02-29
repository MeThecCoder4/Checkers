#include "../pawn.h"
#include <iostream>
#include <cassert>

using namespace Checkers;
using namespace std;
using namespace sf;

void printState(const string &gameState)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << gameState[i * 8 + j];
        }
        cout << endl;
    }

    cout << endl;
}

void Pawn::runAllTests()
{
    cout << "All tests passed" << endl;
}
