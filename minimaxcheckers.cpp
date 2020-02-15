#include "minimaxcheckers.h"

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
    std::list<std::string> children;

    return children;
}

std::string MiniMaxCheckers::buildChild(const string& gameState)
{
    return "";
}