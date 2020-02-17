#ifndef MINIMAX_H
#define MINIMAX_H

#include <list>
#include <algorithm>
#include <climits>
#include <utility>

template<typename T>
class MiniMax
{
public:
    std::pair<T, int> search(T gameState, unsigned int depth, bool maximizingPlayer);

protected:
    virtual std::pair<T, int> staticEval(T gameState) = 0;

    virtual std::list<T> buildChildren(T gameState, bool maximizingPlayer) = 0;
};

template<typename T>
std::pair<T, int> MiniMax<T>::search(T gameState, unsigned int depth, bool maximizingPlayer)
{
    if(depth == 0)
        return staticEval(gameState);
    
    if(maximizingPlayer)
    {
        int maxEvaluation = INT_MIN;
        T maxState = gameState;
        std::list<T> children = buildChildren(gameState, true);

        for(const auto& child : children)
        {
            std::pair<T, int> currentEvaluation = search(child, depth - 1, false);
            maxEvaluation = std::max<int>(maxEvaluation, currentEvaluation.second);

            if(maxEvaluation == currentEvaluation.second)
                maxState = child;
        }

        
        return std::pair<T, int>(maxState, maxEvaluation);
    }
    else
    {
        int minEvaluation = INT_MAX;
        T minState = gameState;
        std::list<T> children = buildChildren(gameState, false);

        for(const auto& child : children)
        {
            std::pair<T, int> currentEvaluation = search(child, depth - 1, true);
            minEvaluation = std::min<int>(minEvaluation, currentEvaluation.second);

            if(minEvaluation == currentEvaluation.second)
                minState = child;
        }

        return std::pair<T, int>(minState, minEvaluation);
    }
}

#endif