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
    std::pair<T, long> search(T gameState, unsigned int depth,
                             long alpha, long beta, bool maximizingPlayer);

protected:
    virtual std::pair<T, long> staticEval(T gameState) = 0;

    virtual std::list<T> buildChildren(T gameState, bool maximizingPlayer) = 0;
};

template<typename T>
std::pair<T, long> MiniMax<T>::search(T gameState, unsigned int depth,
                                     long alpha, long beta, bool maximizingPlayer)
{
    if(depth == 0)
        return staticEval(gameState);
    
    if(maximizingPlayer)
    {
        long maxEvaluation = alpha;
        T maxState = gameState;
        std::list<T> children = buildChildren(gameState, true);

        for(const auto& child : children)
        {
            std::pair<T, long> currentEvaluation = search(child, depth - 1, alpha, beta, false);
            maxEvaluation = std::max<long>(maxEvaluation, currentEvaluation.second);
            alpha = std::max<long>(alpha, maxEvaluation);

            if(maxEvaluation == currentEvaluation.second)
                maxState = child;

            if(beta <= alpha)
                break;
        }

        
        return std::pair<T, long>(maxState, maxEvaluation);
    }
    else
    {
        long minEvaluation = beta;
        T minState = gameState;
        std::list<T> children = buildChildren(gameState, false);

        for(const auto& child : children)
        {
            std::pair<T, long> currentEvaluation = search(child, depth - 1, alpha, beta, true);
            minEvaluation = std::min<long>(minEvaluation, currentEvaluation.second);
            beta = std::min<long>(beta, minEvaluation);

            if(minEvaluation == currentEvaluation.second)
                minState = child;

            if(beta <= alpha)
                break;
        }

        return std::pair<T, long>(minState, minEvaluation);
    }
}

#endif