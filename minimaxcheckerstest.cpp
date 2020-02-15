#include "minimaxcheckers.h"
#include <iostream>

using namespace std;

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
    cout << "Expected: 2" << endl << endl;
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
    cout << "Expected: 0" << endl << endl;
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
    cout << "Expected: -4" << endl << endl;
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
    catch(const char* e)
    {
        cout << e << endl << endl;
    }
}