#include <iostream>
#include "minimaxcheckers.h"

using namespace std;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4');
    // mmc.testStaticEval();
    // mmc.testToIndex();
    // mmc.testIsOnBoard();
    // mmc.testIsFieldEmpty();
    // mmc.testCalcCaptureCoords();
    // mmc.testPawnCapture();
    // mmc.testPawnMove();
    mmc.testBuildFieldChildren();
    cout << "All tests passed." << endl;
    return 0;
}