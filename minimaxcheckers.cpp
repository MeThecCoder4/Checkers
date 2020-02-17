#include "minimaxcheckers.h"
#include <iostream>

using namespace std;

MiniMaxCheckers::MiniMaxCheckers(const char emptyField,
                             const char cpuPawn,
                             const char playerPawn,
                             const char cpuCrownhead,
                             const char playerCrownhead,
                             const uint8_t crownheadFactor, 
                             const uint8_t pawnFactor)
    : m_crownheadFactor(crownheadFactor),
      m_pawnFactor(pawnFactor)
{
    m_figures.emptyField = emptyField;
    m_figures.cpuPawn = cpuPawn;
    m_figures.playerPawn = playerPawn;
    m_figures.cpuCrownhead = cpuCrownhead;
    m_figures.playerCrownhead = playerCrownhead;
}

std::pair<std::string, int> MiniMaxCheckers::staticEval(std::string gameState) 
{
    if(gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";
    
    const int boardSize = m_boardEdge * m_boardEdge;
    int playerPoints = 0, cpuPoints = 0;

    for(const char& field : gameState)
    {
        if(field == m_figures.playerPawn)
            playerPoints += m_pawnFactor;
        else if(field == m_figures.playerCrownhead)
            playerPoints += m_crownheadFactor;
        else if(field == m_figures.cpuPawn)
            cpuPoints += m_pawnFactor;
        else if(field == m_figures.cpuCrownhead)
            cpuPoints += m_crownheadFactor;
    }

    return pair<string, int>(gameState, cpuPoints - playerPoints);
}

std::list<std::string> MiniMaxCheckers::buildChildren(std::string gameState, bool maximizingPlayer)
{
    if(gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    std::list<std::string> children;
    FieldCoords coords;
    char pawn = m_figures.playerPawn, crownhead = m_figures.playerCrownhead;

    if(maximizingPlayer)
    {
        pawn = m_figures.cpuPawn;
        crownhead = m_figures.cpuCrownhead;
    }

    for(coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
        if(coords.y % 2 == 0)
            coords.x = 1;
        else
            coords.x = 0;

        for(;coords.x < m_boardEdge; coords.x += 2)
        {
            if(gameState[coords.toIndex()] == pawn ||
               gameState[coords.toIndex()] == crownhead)
            {
                pair<list<string>, bool> currentFieldChildren = buildFieldChildren(gameState, coords);

                // There was a capture, forget all other states!
                if(currentFieldChildren.second)
                    return currentFieldChildren.first;

                if(currentFieldChildren.first.size() > 0)
                {
                    for(const auto& child : currentFieldChildren.first)
                        children.emplace_back(child);
                }
            }
        }
    }

    // "Normal" flow, no captures
    return children;
}

pair<std::list<std::string>, bool> MiniMaxCheckers::buildFieldChildren(const string& gameState,
                                                           const FieldCoords& coords)
{   
    list<string> fieldChildStates;
    list<FieldCoords> visited;
    
    // First off, build all capture states
    for(uint8_t i = 0; i < 4; i++)
    {
        string childState = pawnCapture(gameState, coords, visited);

        if(childState != gameState)
            fieldChildStates.emplace_back(childState);
    }
    
    if(fieldChildStates.size() > 0)
        return pair<list<string>, bool>(fieldChildStates, true);
    // Only if there is no possibility of capturing
    else
    {
        list<string> moveChildren = pawnMove(gameState, coords);

        for(const auto& child : moveChildren)
            fieldChildStates.emplace_back(child);

        return pair<list<string>, bool>(fieldChildStates, false);
    }
}

std::string MiniMaxCheckers::pawnCapture(const std::string& gameState,
                                         const FieldCoords& coords,
                                         list<FieldCoords>& visited)
{    
    string childState = gameState;
    char figure = gameState[coords.toIndex()];

    char opponentPawn = (figure == m_figures.cpuPawn || figure == m_figures.cpuCrownhead) ?
                                                        m_figures.playerPawn : m_figures.cpuPawn;

    char opponentCrownhead = (figure == m_figures.cpuPawn || figure == m_figures.cpuCrownhead) ? 
                                                        m_figures.playerCrownhead : m_figures.cpuCrownhead;
    FieldCoords neighs;                                                              
    
    for(neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y += 2)
    {
        for(neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
        {
            if(neighs.isOnBoard())
            {
                // If current neighbour field wasn't visited before
                if(find_if(visited.begin(), visited.end(), 
                [&neighs](FieldCoords current)
                {
                    return current.y == neighs.y && current.x == neighs.x;
                }) == visited.end())
                {
                    if(childState[neighs.toIndex()] == opponentPawn ||
                        childState[neighs.toIndex()] == opponentCrownhead)
                    {
                        // captureCoords are not coordinates of an opponent,
                        // but the coordinates of a field behind it (diagonally)
                        FieldCoords captureCoords = calcCaptureCoords(coords, neighs);

                        if(captureCoords.isOnBoard())
                        {
                            if(isFieldEmpty(childState, captureCoords))
                            {
                                char opponent = childState[neighs.toIndex()];
                                childState[neighs.toIndex()] = m_figures.emptyField;
                                childState[captureCoords.toIndex()] = figure;
                                childState[coords.toIndex()] = m_figures.emptyField;
                                visited.emplace_back(neighs);
                                return childState;
                            }
                        }
                    }
                }
            }
        }
    }

    return gameState;
}

MiniMaxCheckers::FieldCoords MiniMaxCheckers::calcCaptureCoords(const FieldCoords& myCoords,
                                                                const FieldCoords& opponentCoords)
{
    FieldCoords opponentDirection;
    opponentDirection.y = opponentCoords.y - myCoords.y;
    opponentDirection.x = opponentCoords.x - myCoords.x;
    FieldCoords captureCoords;
    captureCoords.y = myCoords.y + (2 * opponentDirection.y);
    captureCoords.x = myCoords.x + (2 * opponentDirection.x);
    return captureCoords;
}

bool MiniMaxCheckers::isFieldEmpty(const std::string& gameState, const FieldCoords& coords)
{
    return gameState[coords.toIndex()] == m_figures.emptyField;
}

list<string> MiniMaxCheckers::pawnMove(const string& gameState,
                                       const FieldCoords& coords)
{
    char pawn = gameState[coords.toIndex()];
    list<string> resultStates;

    if(pawn != m_figures.cpuPawn && pawn != m_figures.playerPawn)
        return list<string>();
    
    FieldCoords neighs[2];

    // Cpu always goes up from its perspective, player goes down the board 
    if(pawn == m_figures.cpuPawn)
    {
        neighs[0].y = neighs[1].y = coords.y - 1;
        neighs[0].x = coords.x - 1;
        neighs[1].x = coords.x + 1;
    }
    else
    {
        neighs[0].y = neighs[1].y = coords.y + 1;
        neighs[0].x = coords.x - 1;
        neighs[1].x = coords.x + 1;
    }

    for(int8_t i = 0; i < 2; i++)
    {
        if(neighs[i].isOnBoard() && isFieldEmpty(gameState, neighs[i]))
        {
            string childState = gameState;
            childState[coords.toIndex()] = m_figures.emptyField;
            childState[neighs[i].toIndex()] = pawn;
            resultStates.emplace_back(childState);
        }
    }

    return resultStates;
}