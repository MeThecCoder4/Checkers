#ifndef MINIMAX_H
#define MINIMAX_H

#include <list>
#include <algorithm>
#include <climits>

template<typename T>
class MiniMax
{
public:
    int search(T gameState, unsigned int depth, bool maximizingPlayer);

protected:
    virtual int staticEval(T gameState, bool maximizingPlayer) = 0;

    virtual std::list<T> buildChildren(T gameState) = 0;

};

template<typename T>
int MiniMax<T>::search(T gameState, unsigned int depth, bool maximizingPlayer)
{
    if(depth == 0)
        return staticEval(gameState, maximizingPlayer);
    
    if(maximizingPlayer)
    {
        int maxEvaluation = INT_MIN;
        std::list<T> children = buildChildren(gameState);

        for(const auto& child : children)
        {
            int currentEvaluation = search(child, depth - 1, false);
            maxEvaluation = std::max<int>(maxEvaluation, currentEvaluation);
        }

        return maxEvaluation;
    }
    else
    {
        int minEvaluation = INT_MAX;
        std::list<T> children = buildChildren(gameState);

        for(const auto& child : children)
        {
            int currentEvaluation = search(child, depth - 1, true);
            minEvaluation = std::min<int>(minEvaluation, currentEvaluation);
        }

        return minEvaluation;
    }
}

#endif