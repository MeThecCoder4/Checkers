#include "minimaxcheckers.h"
#include <iostream>
#include <cmath>

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

long MiniMaxCheckers::pointsForSafety(const std::string &gameState)
{
    long playerPoints = 0, cpuPoints = 0;
    FieldCoords coords;

    for (coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
        for (coords.x = 0; coords.x < m_boardEdge; coords.x++)
        {
            int penalty = 40;
            const char figure = gameState[coords.toIndex()];
            pair<char, char> opponents = getOpponentFigures(figure);
            FieldCoords neighFields[4] = {FieldCoords(coords.y - 1, coords.x - 1),
                                          FieldCoords(coords.y - 1, coords.x + 1),
                                          FieldCoords(coords.y + 1, coords.x - 1),
                                          FieldCoords(coords.y + 1, coords.x + 1)};

            if (figure == m_figures.cpuPawn)
            {
                if (neighFields[0].isOnBoard() && neighFields[3].isOnBoard())
                {
                    if ((gameState[neighFields[0].toIndex()] == opponents.first ||
                         gameState[neighFields[0].toIndex()] == opponents.second) &&
                        isFieldEmpty(gameState, neighFields[3]))
                        playerPoints += penalty;
                }

                if (neighFields[1].isOnBoard() && neighFields[2].isOnBoard())
                {
                    if ((gameState[neighFields[1].toIndex()] == opponents.first ||
                         gameState[neighFields[1].toIndex()] == opponents.second) &&
                        isFieldEmpty(gameState, neighFields[2]))
                        playerPoints += penalty;
                }
            }
            else if (figure == m_figures.playerPawn)
            {
                if (neighFields[0].isOnBoard() && neighFields[3].isOnBoard())
                {
                    if ((gameState[neighFields[3].toIndex()] == opponents.first ||
                         gameState[neighFields[3].toIndex()] == opponents.second) &&
                        isFieldEmpty(gameState, neighFields[0]))
                        cpuPoints += penalty;
                }

                if (neighFields[1].isOnBoard() && neighFields[2].isOnBoard())
                {
                    if ((gameState[neighFields[2].toIndex()] == opponents.first ||
                         gameState[neighFields[2].toIndex()] == opponents.second) &&
                        isFieldEmpty(gameState, neighFields[1]))
                        cpuPoints += penalty;
                }
            }
        }
    }

    return cpuPoints - playerPoints;
}

long MiniMaxCheckers::pointsForFigures(const std::string &gameState)
{
    long playerPoints = 0, cpuPoints = 0;
    FieldCoords coords;

    for (coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
        for (coords.x = 0; coords.x < m_boardEdge; coords.x++)
        {
            const char figure = gameState[coords.toIndex()];

            if (figure == m_figures.playerPawn)
            {
                playerPoints += m_pawnFactor;
            }
            else if (figure == m_figures.playerCrownhead)
            {
                playerPoints += m_crownheadFactor;
            }
            else if (figure == m_figures.cpuPawn)
            {
                cpuPoints += m_pawnFactor;
            }
            else if (figure == m_figures.cpuCrownhead)
            {
                cpuPoints += m_crownheadFactor;
            }
        }
    }

    if (playerPoints == 0)
        return LONG_MAX;

    if (cpuPoints == 0)
        return LONG_MIN;

    return cpuPoints - playerPoints;
}

std::pair<std::string, long> MiniMaxCheckers::staticEval(std::string gameState)
{
    if (gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    long points = pointsForFigures(gameState);

    if (points == LONG_MAX || points == LONG_MIN)
        return pair<string, long>(gameState, points);

    return pair<string, long>(gameState, points);
}

std::list<std::string> MiniMaxCheckers::buildChildren(std::string gameState, bool maximizingPlayer)
{
    if (gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    list<string> captures;
    list<string> moves;
    FieldCoords coords;
    char pawn = m_figures.playerPawn, crownhead = m_figures.playerCrownhead;

    if (maximizingPlayer)
    {
        pawn = m_figures.cpuPawn;
        crownhead = m_figures.cpuCrownhead;
    }

    for (coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
        // Valid fields only
        if (coords.y % 2 == 0)
            coords.x = 1;
        else
            coords.x = 0;

        for (; coords.x < m_boardEdge; coords.x += 2)
        {
            if (gameState[coords.toIndex()] == pawn ||
                gameState[coords.toIndex()] == crownhead)
            {
                pair<list<string>, bool> currentFieldChildren = buildFieldChildren(gameState,
                                                                                   coords,
                                                                                   maximizingPlayer);

                for (auto &child : currentFieldChildren.first)
                    makeCrownheads(child);

                if (currentFieldChildren.second)
                {
                    for (const auto &child : currentFieldChildren.first)
                        captures.emplace_back(child);
                }
                else
                {
                    for (const auto &child : currentFieldChildren.first)
                        moves.emplace_back(child);
                }
            }
        }
    }

    return (captures.size() > 0) ? captures : moves;
}

pair<std::list<std::string>, bool> MiniMaxCheckers::buildFieldChildren(const string &gameState,
                                                                       const FieldCoords &coords,
                                                                       bool maximizingPlayer)
{
    list<string> fieldChildStates;
    bool isPawn = (gameState[coords.toIndex()] == m_figures.cpuPawn ||
                   gameState[coords.toIndex()] == m_figures.playerPawn)
                      ? true
                      : false;

    // First off, build all capture states
    list<string> childStates;

    if (isPawn)
        childStates = pawnCapture(gameState, coords, maximizingPlayer);
    else
        childStates = crownheadCapture(gameState, coords, maximizingPlayer);

    for (const auto &child : childStates)
        if (child != gameState)
            fieldChildStates.emplace_back(child);

    if (fieldChildStates.size() > 0)
        return pair<list<string>, bool>(fieldChildStates, true);

    list<string> moveChildren;

    if (isPawn)
        moveChildren = pawnMove(gameState, coords);
    else
        moveChildren = crownheadMove(gameState, coords);

    for (const auto &child : moveChildren)
        if (child != gameState)
            fieldChildStates.emplace_back(child);

    return pair<list<string>, bool>(fieldChildStates, false);
}

list<std::string> MiniMaxCheckers::pawnCapture(const std::string &gameState,
                                               const FieldCoords &coords,
                                               bool maximizingPlayer)
{
    char figure = gameState[coords.toIndex()];
    list<string> resultStates;

    if (figure == m_figures.cpuCrownhead || figure == m_figures.playerCrownhead)
        return list<string>();

    pair<char, char> opponentFigures = getOpponentFigures(figure);
    char opponentPawn, opponentCrownhead;
    opponentPawn = opponentFigures.first;
    opponentCrownhead = opponentFigures.second;
    FieldCoords neighs(coords.y - 1, coords.x - 1);

    if (opponentPawn == m_figures.cpuPawn)
        neighs.y = coords.y + 1;

    // For both neighbour fields
    for (neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
    {
        string childState = gameState;

        if (neighs.isOnBoard())
        {
            if (childState[neighs.toIndex()] == opponentPawn ||
                childState[neighs.toIndex()] == opponentCrownhead)
            {
                // captureCoords are not coordinates of an opponent,
                // but the coordinates of a field behind it (diagonally)
                FieldCoords captureCoords = calcCaptureCoords(coords, neighs);

                if (captureCoords.isOnBoard())
                {
                    if (isFieldEmpty(childState, captureCoords))
                    {
                        updateCaptureState(childState, neighs, coords, captureCoords, figure);
                        m_captureStates.emplace_back(childState);

                        // Handle multi captures with recursion
                        pair<string, long> bestState = staticEval(childState);
                        list<string> nextChildStates = pawnCapture(childState,
                                                                   captureCoords,
                                                                   maximizingPlayer);
                        bestState = getNewBestState(nextChildStates, bestState, maximizingPlayer);
                        resultStates.emplace_back(bestState.first);
                    }
                }
            }
        }
    }

    return (resultStates.size() > 0) ? resultStates : list<string>();
}

list<std::string> MiniMaxCheckers::crownheadCapture(const std::string &gameState,
                                                    const FieldCoords &coords,
                                                    bool maximizingPlayer)
{
    char figure = gameState[coords.toIndex()];
    list<string> resultStates;

    if (figure == m_figures.cpuPawn || figure == m_figures.playerPawn)
        return list<string>();

    pair<char, char> opponentFigures = getOpponentFigures(figure);
    char opponentPawn, opponentCrownhead;
    opponentPawn = opponentFigures.first;
    opponentCrownhead = opponentFigures.second;
    FieldCoords neighs;

    for (neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y += 2)
    {
        for (neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
        {
            string childState = gameState;

            if (neighs.isOnBoard())
            {
                if (childState[neighs.toIndex()] == opponentPawn ||
                    childState[neighs.toIndex()] == opponentCrownhead)
                {
                    // captureCoords are not coordinates of an opponent,
                    // but the coordinates of a field behind it (diagonally)
                    FieldCoords captureCoords = calcCaptureCoords(coords, neighs);

                    if (captureCoords.isOnBoard())
                    {
                        if (isFieldEmpty(childState, captureCoords))
                        {
                            updateCaptureState(childState, neighs, coords, captureCoords, figure);
                            m_captureStates.emplace_back(childState);

                            // Handle multi captures with recursion
                            pair<string, long> bestState = staticEval(childState);
                            list<string> nextChildStates = crownheadCapture(childState,
                                                                            captureCoords,
                                                                            maximizingPlayer);
                            bestState = getNewBestState(nextChildStates, bestState, maximizingPlayer);
                            resultStates.emplace_back(bestState.first);
                        }
                    }
                }
            }
        }
    }

    return (resultStates.size() > 0) ? resultStates : list<string>();
}

MiniMaxCheckers::FieldCoords MiniMaxCheckers::calcCaptureCoords(const FieldCoords &myCoords,
                                                                const FieldCoords &opponentCoords)
{
    FieldCoords opponentDirection;
    opponentDirection.y = opponentCoords.y - myCoords.y;
    opponentDirection.x = opponentCoords.x - myCoords.x;
    FieldCoords captureCoords;
    captureCoords.y = myCoords.y + (2 * opponentDirection.y);
    captureCoords.x = myCoords.x + (2 * opponentDirection.x);
    return captureCoords;
}

bool MiniMaxCheckers::isFieldEmpty(const std::string &gameState, const FieldCoords &coords)
{
    if (coords.isOnBoard())
        return gameState[coords.toIndex()] == m_figures.emptyField;

    return false;
}

void MiniMaxCheckers::makeCrownheads(std::string &gameState)
{
    FieldCoords coords;

    for (coords.x = 0; coords.x < m_boardEdge; coords.x++)
    {
        coords.y = 0;

        if (gameState[coords.toIndex()] == m_figures.cpuPawn)
            gameState[coords.toIndex()] = m_figures.cpuCrownhead;

        coords.y = m_boardEdge - 1;

        if (gameState[coords.toIndex()] == m_figures.playerPawn)
            gameState[coords.toIndex()] = m_figures.playerCrownhead;
    }
}

std::list<std::string> MiniMaxCheckers::crownheadMove(const std::string &gameState,
                                                      const FieldCoords &coords)
{
    char crownhead = gameState[coords.toIndex()];
    list<string> resultStates;

    if (crownhead != m_figures.cpuCrownhead && crownhead != m_figures.playerCrownhead)
        return list<string>();

    FieldCoords neighs;

    for (neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y += 2)
    {
        for (neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
        {
            string childState = gameState;

            if (neighs.isOnBoard() && isFieldEmpty(childState, neighs))
            {
                childState[neighs.toIndex()] = crownhead;
                childState[coords.toIndex()] = m_figures.emptyField;
                resultStates.emplace_back(childState);
            }
        }
    }

    return resultStates;
}

list<string> MiniMaxCheckers::pawnMove(const string &gameState,
                                       const FieldCoords &coords)
{
    char pawn = gameState[coords.toIndex()];
    list<string> resultStates;

    if (pawn != m_figures.cpuPawn && pawn != m_figures.playerPawn)
        return list<string>();

    FieldCoords neighs[2];

    // Cpu always goes up from its perspective, player goes down the board
    if (pawn == m_figures.cpuPawn)
        neighs[0].y = neighs[1].y = coords.y - 1;
    else
        neighs[0].y = neighs[1].y = coords.y + 1;

    neighs[0].x = coords.x - 1;
    neighs[1].x = coords.x + 1;

    for (int8_t i = 0; i < 2; i++)
    {
        if (neighs[i].isOnBoard() && isFieldEmpty(gameState, neighs[i]))
        {
            string childState = gameState;
            childState[coords.toIndex()] = m_figures.emptyField;
            childState[neighs[i].toIndex()] = pawn;

            // This condition fixes a bug, without it the computer starts acting like an idiot
            if ((pawn == m_figures.cpuPawn && pointsForSafety(childState) >= -m_pawnFactor) ||
                (pawn == m_figures.playerPawn && pointsForSafety(childState) <= m_pawnFactor))
                resultStates.emplace_back(childState);
        }
    }

    return resultStates;
}

void MiniMaxCheckers::updateCaptureState(std::string &gameState, const FieldCoords &opponentCoords,
                                         const FieldCoords &myCurrentCoords,
                                         const FieldCoords &myNextCoords,
                                         char myFigure)
{
    unsigned int opponentIndex = opponentCoords.toIndex();
    char opponent = gameState[opponentIndex];

    gameState[opponentIndex] = m_figures.emptyField;
    gameState[myNextCoords.toIndex()] = myFigure;
    gameState[myCurrentCoords.toIndex()] = m_figures.emptyField;
}

std::pair<char, char> MiniMaxCheckers::getOpponentFigures(char figure)
{
    std::pair<MiniMaxCheckers::Figures, MiniMaxCheckers::Figures> opponentFigures;
    char opponentPawn, opponentCrownhead;

    if (figure == m_figures.cpuPawn || figure == m_figures.playerPawn)
    {
        opponentPawn = (figure == m_figures.cpuPawn) ? m_figures.playerPawn : m_figures.cpuPawn;
        opponentCrownhead = (figure == m_figures.cpuPawn) ? m_figures.playerCrownhead : m_figures.cpuCrownhead;
    }
    else
    {
        opponentPawn = (figure == m_figures.cpuCrownhead) ? m_figures.playerPawn : m_figures.cpuPawn;
        opponentCrownhead = (figure == m_figures.cpuCrownhead) ? m_figures.playerCrownhead : m_figures.cpuCrownhead;
    }

    return pair<char, char>(opponentPawn, opponentCrownhead);
}

std::pair<std::string, long> MiniMaxCheckers::getNewBestState(const std::list<std::string> &nextStates,
                                                              std::pair<std::string, long> currentBest,
                                                              bool maximizingPlayer)
{
    for (const auto &next : nextStates)
    {
        pair<string, long> nextEval = staticEval(next);

        if (maximizingPlayer)
        {
            if (nextEval.second > currentBest.second)
                currentBest.first = nextEval.first;
        }
        else
        {
            if (nextEval.second < currentBest.second)
                currentBest.first = nextEval.first;
        }
    }

    return currentBest;
}

std::list<std::string> MiniMaxCheckers::getCaptureStates()
{
    return m_captureStates;
}