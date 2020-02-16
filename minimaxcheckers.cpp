#include "minimaxcheckers.h"
#include <set>

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

std::list<std::string> MiniMaxCheckers::buildChildren(std::string gameState)
{
    if(gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    std::list<std::string> children;

    return children;
}

std::list<std::string> MiniMaxCheckers::buildFieldChildren(const string& gameState,
                                                           const FieldCoords& coords)
{   
    list<string> fieldChildStates;
    list<FieldCoords> visited;

    return fieldChildStates;
}

std::string MiniMaxCheckers::pawnCapture(const std::string& gameState,
                                         const FieldCoords& coords,
                                         const list<FieldCoords>& visited)
{    
    string childState = gameState;
    char figure = gameState[coords.toIndex()];

    char opponentPawn = (figure == m_figures.cpuPawn || figure == m_figures.cpuCrownhead) ?
                                                        m_figures.playerPawn : m_figures.cpuPawn;

    char opponentCrownhead = (figure == m_figures.cpuPawn || figure == m_figures.cpuCrownhead) ? 
                                                        m_figures.playerCrownhead : m_figures.cpuCrownhead;
    FieldCoords neighs;                                                              
    
    for(neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y++)
    {
        for(neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x++)
        {
            if((neighs.y != coords.y || neighs.x != coords.x))
            {
                if(neighs.isOnBoard() && coords.isDiagonalTo(neighs))
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
                            FieldCoords captureCoords = calcCaptureCoords(coords, neighs);

                            if(captureCoords.isOnBoard())
                            {
                                if(isFieldEmpty(childState, captureCoords))
                                {
                                    char opponent = childState[neighs.toIndex()];
                                    childState[neighs.toIndex()] = m_figures.emptyField;
                                    childState[captureCoords.toIndex()] = figure;
                                    childState[coords.toIndex()] = m_figures.emptyField;
                                    return childState;
                                }
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