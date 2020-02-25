#include "../minimaxcheckers.h"
#include <iostream>

using namespace std;

void MiniMaxCheckers::runAllTests()
{
    testStaticEval();

    testToIndex();

    testIsOnBoard();

    testIsFieldEmpty();

    testCalcCaptureCoords();

    testIsDiagonalTo();

    testPawnCapture();

    testPawnMove();

    testBuildFieldChildren();

    cout << "All tests passed" << endl;
}

void MiniMaxCheckers::printState(const string &gameState)
{
    for (int i = 0; i < m_boardEdge; i++)
    {
        for (int j = 0; j < m_boardEdge; j++)
        {
            cout << gameState[i * m_boardEdge + j];
        }
        cout << endl;
    }
}

void MiniMaxCheckers::testStaticEval()
{
    string state1 = "00000000"
                    "00010000"
                    "00001000"
                    "00010000"
                    "00002000"
                    "00200000"
                    "20003000"
                    "00000000";

    cout << "staticEval(): " << staticEval(state1).second << endl;
    cout << "Expected: 2" << endl
         << endl;
    assert(staticEval(state1).second == 2);

    string state2 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    cout << "staticEval(): " << staticEval(state2).second << endl;
    cout << "Expected: 0" << endl
         << endl;
    assert(staticEval(state2).second == 0);

    string state3 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000010"
                    "00000000"
                    "00000444";

    cout << "staticEval(): " << staticEval(state3).second << endl;
    cout << "Expected: -4" << endl
         << endl;
    assert(staticEval(state3).second == -4);

    string state4 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000010"
                    "00000000"
                    "0000044";

    try
    {
        staticEval(state4);
    }
    catch (const char *e)
    {
        cout << e << endl
             << endl;
    }

    string state5 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00020000"
                    "40000000";

    assert(staticEval(state5).second == -3);
}

void MiniMaxCheckers::testToIndex()
{
    FieldCoords coords1 = {2, 3};
    assert(coords1.toIndex() == 19);

    FieldCoords coords2 = {0, 0};
    assert(coords2.toIndex() == 0);

    FieldCoords coords3 = {7, 7};
    assert(coords3.toIndex() == 63);

    try
    {
        FieldCoords coords4 = {8, 2};
        coords4.toIndex();
    }
    catch (const char *e)
    {
        std::cout << e << '\n';
    }
}

void MiniMaxCheckers::testIsOnBoard()
{
    FieldCoords coords1 = {2, 3};
    assert(coords1.isOnBoard() == true);

    FieldCoords coords2 = {0, 0};
    assert(coords2.isOnBoard() == true);

    FieldCoords coords3 = {7, 7};
    assert(coords3.isOnBoard() == true);

    FieldCoords coords4 = {8, 2};
    assert(coords4.isOnBoard() == false);

    FieldCoords coords5 = {-1, -1};
    assert(coords5.isOnBoard() == false);
}

void MiniMaxCheckers::testIsFieldEmpty()
{
    string state1 = "00000000"
                    "00010000"
                    "00001000"
                    "00010000"
                    "00002000"
                    "00200000"
                    "20003000"
                    "00000000";

    FieldCoords coords1 = {2, 3};
    assert(isFieldEmpty(state1, coords1) == true);

    FieldCoords coords2 = {1, 3};
    assert(isFieldEmpty(state1, coords2) == false);

    FieldCoords coords3 = {6, 0};
    assert(isFieldEmpty(state1, coords3) == false);

    try
    {
        FieldCoords coords4 = {8, 2};
        isFieldEmpty(state1, coords4);
    }
    catch (const char *e)
    {
        std::cerr << e << '\n';
    }
}

void MiniMaxCheckers::testCalcCaptureCoords()
{
    // Test board example
    // "00000000"
    // "00000000"
    // "00000200"
    // "00001000"
    // "00000000"
    // "00000000"
    // "00000000"
    // "00000000"

    FieldCoords captureCoords1 = calcCaptureCoords({3, 4}, {2, 5});
    cout << "Calculated capture coords: (" + to_string(captureCoords1.x) << ", " << to_string(captureCoords1.y) + ')' << endl;
    assert(captureCoords1.y == 1 && captureCoords1.x == 6);

    FieldCoords captureCoords2 = calcCaptureCoords({2, 5}, {3, 4});
    assert(captureCoords2.y == 4 && captureCoords2.x == 3);
}

void MiniMaxCheckers::testIsDiagonalTo()
{
    string state1 = "00000000"
                    "00000000"
                    "00000000"
                    "22200000"
                    "21200000"
                    "22200000"
                    "00000000"
                    "00000000";

    FieldCoords coords1 = {4, 1};
    assert(coords1.isDiagonalTo({4, 0}) == false);

    assert(coords1.isDiagonalTo({4, 2}) == false);

    assert(coords1.isDiagonalTo({3, 2}) == true);

    assert(coords1.isDiagonalTo({4, 1}) == false);

    assert(coords1.isDiagonalTo({5, 0}) == true);
}

void MiniMaxCheckers::testPawnCapture()
{
    string state1 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    assert(pawnCapture(state1, {3, 4}, true) == list<string>());

    assert(pawnCapture(state1, {0, 0}, true) == list<string>());

    string state2 = "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00010000"
                    "00022000"
                    "00000000";

    assert(pawnCapture(state2, {3, 4}, true) == list<string>());

    string state3 = "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00000000"
                    "00020000"
                    "00000100";

    list<string> comp1 = {state3};
    assert(pawnCapture(state2, {5, 3}, true) == comp1);

    string state4 = "00000000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00003000"
                    "00010000"
                    "00020000"
                    "00000000";

    assert(pawnCapture(state4, {5, 3}, true) == list<string>());

    string state5 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00004000"
                    "00010000"
                    "00000000"
                    "00000000";

    string state6 = "00000000"
                    "00000000"
                    "00000000"
                    "00000100"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    list<string> comp2 = {state6};
    assert(pawnCapture(state5, {5, 3}, true) == comp2);

    string state7 = "00000000"
                    "00000000"
                    "00000000"
                    "00000100"
                    "00002000"
                    "00010000"
                    "00000000"
                    "00000000";

    assert(pawnCapture(state7, {5, 3}, true) == list<string>());

    string state8 = "00000000"
                    "00000000"
                    "00000000"
                    "00000300"
                    "00002000"
                    "00010000"
                    "00000000"
                    "00000000";

    assert(pawnCapture(state8, {5, 3}, true) == list<string>());

    string state9 = "20000000"
                    "01000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    assert(pawnCapture(state9, {1, 1}, true) == list<string>());

    string state10 = "02020000"
                     "20100000"
                     "00000202"
                     "00000000"
                     "00000100"
                     "10101000"
                     "00000000"
                     "00000000";

    string state11 = "00020000"
                     "20000000"
                     "00020202"
                     "00000000"
                     "00000100"
                     "10101000"
                     "00000000"
                     "00000000";

    list<string> comp3 = {state11};
    assert(pawnCapture(state10, {0, 1}, false) == comp3);
}

void MiniMaxCheckers::testPawnMove()
{
    string state1 = "00000000"
                    "00000000"
                    "00020000"
                    "00000000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00000000";

    string state2 = "00000000"
                    "00000000"
                    "00000000"
                    "00200000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00000000";

    string state3 = "00000000"
                    "00000000"
                    "00000000"
                    "00002000"
                    "00000000"
                    "00001000"
                    "00000000"
                    "00000000";

    list<string> compareStates1 = {state2, state3};
    list<string> resultStates1 = pawnMove(state1, {2, 3});
    assert(resultStates1 == compareStates1);

    string state4 = "00000000"
                    "00000000"
                    "00020000"
                    "00000000"
                    "00010000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state5 = "00000000"
                    "00000000"
                    "00020000"
                    "00000000"
                    "00000100"
                    "00000000"
                    "00000000"
                    "00000000";

    list<string> compareStates2 = {state4, state5};
    list<string> resultStates2 = pawnMove(state1, {5, 4});
    assert(resultStates2 == compareStates2);

    string state6 = "00010000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    assert(pawnMove(state6, {0, 3}) == list<string>());

    string state7 = "00000000"
                    "00200000"
                    "00010000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state8 = "00000000"
                    "00201000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    list<string> compareStates3 = {state8};
    assert(pawnMove(state7, {2, 3}) == compareStates3);

    string state9 = "00000020"
                    "00000001"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    assert(pawnMove(state9, {1, 7}) == list<string>());

    string state10 = "00000000"
                     "00000000"
                     "00000000"
                     "00030000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00000000";

    assert(pawnMove(state10, {3, 3}) == list<string>());
}

void MiniMaxCheckers::testBuildFieldChildren()
{
    string state1 = "00000000"
                    "00000000"
                    "00010000"
                    "00002000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state2 = "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000100"
                    "00000000"
                    "00000000"
                    "00000000";

    pair<list<string>, bool> comparisonStates1 = {{state2}, true};
    assert(buildFieldChildren(state1, {2, 3}, true) == comparisonStates1);

    string state3 = "00000000"
                    "00000000"
                    "00010000"
                    "00402000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state4 = "00000000"
                    "00000000"
                    "00000000"
                    "00002000"
                    "01000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state5 = "00000000"
                    "00000000"
                    "00000000"
                    "00400000"
                    "00000100"
                    "00000000"
                    "00000000"
                    "00000000";

    pair<list<string>, bool> comparisonStates2 = {{state4, state5}, true};
    assert(buildFieldChildren(state3, {2, 3}, true) == comparisonStates2);

    string state6 = "00000000"
                    "00000000"
                    "00010000"
                    "00402000"
                    "01000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state7 = "00000000"
                    "00000000"
                    "00000000"
                    "00400000"
                    "01000100"
                    "00000000"
                    "00000000"
                    "00000000";

    pair<list<string>, bool> comparisonStates3 = {{state7}, true};
    assert(buildFieldChildren(state6, {2, 3}, true) == comparisonStates3);

    string state8 = "01000100"
                    "00000000"
                    "00010000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state9 = "01000100"
                    "00100000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000"
                    "00000000";

    string state10 = "01000100"
                     "00001000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00000000";

    pair<list<string>, bool> comparisonStates4 = {{state9, state10}, false};
    assert(buildFieldChildren(state8, {2, 3}, false) == comparisonStates4);

    string state11 = "00000000"
                     "00100000"
                     "00020000"
                     "00000000"
                     "00020000"
                     "00000000"
                     "00000000"
                     "00000000";

    string state12 = "00000000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00000000"
                     "00100000"
                     "00000000"
                     "00000000";

    pair<list<string>, bool> comparisonStates5 = {{state12}, true};
    assert(buildFieldChildren(state11, {1, 2}, true) == comparisonStates5);
}