#include <iostream>
#include "minimaxcheckers.h"

using namespace std;

int main()
{
    MiniMaxCheckers mmc('0', '1', '2', '3', '4');
    mmc.testStaticEval();
    return 0;
}