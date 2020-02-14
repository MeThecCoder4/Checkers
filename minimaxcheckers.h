#ifndef MINIMAXCHECKERS_H
#define MINIMAXCHECKERS_H

#include "minimax.h"

template<typename T>
class MiniMaxCheckers : public MiniMax<T>
{
protected:
    int staticEval(T gameState, bool maximizingPlayer) override;

    std::list<T> buildChildren(T gameState) override;
};

template<typename T>
int MiniMaxCheckers<T>::staticEval(T gameState, bool maximizingPlayer) 
{
    return 0;
}

template<typename T>
std::list<T> MiniMaxCheckers<T>::buildChildren(T gameState)
{
    std::list<T> children;

    return children;
}

#endif