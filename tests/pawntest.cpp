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
    testGetPossibleJumps();
    cout << "All tests passed" << endl;
}

void Pawn::testGetPossibleJumps()
{
    string state1 = "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00020000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state2 = "00000000"
                    "00000000"
                    "00000200"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    select();
    list<pair<Vector2u, string>> comp1 = {{pair<Vector2u, string>({5, 2}, state2)}};
    cout << getPossibleJumps(state1, {3, 4}).size() << endl;
    assert(getPossibleJumps(state1, {3, 4}) == comp1);

    string state3 = "00000000"
                    "00001000"
                    "00000000"
                    "00001000"
                    "00020000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state4 = "00020000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    list<pair<Vector2u, string>> comp2 = {{pair<Vector2u, string>({3, 0}, state4)}};
    assert(getPossibleJumps(state3, {3, 4}) == comp2);

    string state5 = "00000000"
                    "00001000"
                    "00000000"
                    "00001000"
                    "00020000"
                    "00001000"
                    "00000000"
                    "00000000";

    string state6 = "00020000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00000000";

    list<pair<Vector2u, string>> comp3 = {{pair<Vector2u, string>({3, 0}, state6)}};
    cout << getPossibleJumps(state5, {3, 4}).size() << endl;
    assert(getPossibleJumps(state5, {3, 4}) == comp3);
}