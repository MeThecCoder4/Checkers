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

std::pair<std::string, long> MiniMaxCheckers::staticEval(std::string gameState)
{
    if (gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    const int boardSize = m_boardEdge * m_boardEdge;
    long playerPoints = 0, cpuPoints = 0;
    // ADFC - Average Distance From Center
    float playerADFC = 0.0f, cpuADFC = 0.0f;
    int8_t playerCounter = 0, cpuCounter = 0;

    FieldCoords coords;
    FieldCoords center(3, 3);

    for (coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
        for (coords.x = 0; coords.x < m_boardEdge; coords.x++)
        {
            char field = gameState[coords.toIndex()];
            float sumOfSquares = (!isFieldEmpty(gameState, coords)) ? pow(static_cast<float>(coords.y - center.y), 2) +
                                                                          pow(static_cast<float>(coords.x - center.x), 2)
                                                                    : 0.0f;

            if (field == m_figures.playerPawn)
            {
                playerPoints += m_pawnFactor;
                playerADFC += sqrt(sumOfSquares);
                playerCounter++;
            }
            else if (field == m_figures.playerCrownhead)
            {
                playerPoints += m_crownheadFactor;
                playerADFC += sqrt(sumOfSquares);
                playerCounter++;
            }
            else if (field == m_figures.cpuPawn)
            {
                cpuPoints += m_pawnFactor;
                cpuADFC += sqrt(sumOfSquares);
                cpuCounter++;
            }
            else if (field == m_figures.cpuCrownhead)
            {
                cpuPoints += m_crownheadFactor;
                cpuADFC += sqrt(sumOfSquares);
                cpuCounter++;
            }
        }
    }

    float rewardForADFC = 5.0f;

    playerADFC /= playerCounter;
    cpuADFC /= cpuCounter;

    if (playerADFC > cpuADFC)
        playerPoints += rewardForADFC;
    else
        cpuPoints += rewardForADFC;

    return pair<string, long>(gameState, cpuPoints - playerPoints);
}

std::list<std::string> MiniMaxCheckers::buildChildren(std::string gameState, bool maximizingPlayer)
{
    if (gameState.length() != m_boardEdge * m_boardEdge)
        throw "Invalid state length.";

    std::list<std::string> endChildren;
    bool capture = false;
    list<pair<string, bool>> children;
    FieldCoords coords;
    char pawn = m_figures.playerPawn, crownhead = m_figures.playerCrownhead;

    if (maximizingPlayer)
    {
        pawn = m_figures.cpuPawn;
        crownhead = m_figures.cpuCrownhead;
    }

    for (coords.y = 0; coords.y < m_boardEdge; coords.y++)
    {
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
                    capture = true;

                for (const auto &child : currentFieldChildren.first)
                {
                    if (currentFieldChildren.second)
                        children.emplace_back(pair<string, bool>(child, true));
                    else
                        children.emplace_back(pair<string, bool>(child, false));
                }
            }
        }
    }

    if (capture)
    {
        for (const auto &child : children)
        {
            if (child.second)
            {
                m_captureStates.emplace_back(child.first);
                endChildren.emplace_back(child.first);
            }
        }
    }
    else
        for (const auto &child : children)
            endChildren.emplace_back(child.first);

    return endChildren;
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
    {
        auto setOfCaptures = crownheadCapture(gameState, coords, maximizingPlayer);

        for (const auto &e : setOfCaptures)
            childStates.emplace_back(e);
    }

    for (const auto &child : childStates)
        if (child != gameState)
        {
            fieldChildStates.emplace_back(child);
        }

    if (fieldChildStates.size() > 0)
        return pair<list<string>, bool>(fieldChildStates, true);
    // Only if there is no possibility of capturing
    else
    {
        list<string> moveChildren;

        if (isPawn)
            moveChildren = pawnMove(gameState, coords);
        else
            moveChildren = crownheadMove(gameState, coords);

        for (const auto &child : moveChildren)
            fieldChildStates.emplace_back(child);

        return pair<list<string>, bool>(fieldChildStates, false);
    }
}

list<std::string> MiniMaxCheckers::pawnCapture(const std::string &gameState,
                                               const FieldCoords &coords,
                                               bool maximizingPlayer)
{
    char figure = gameState[coords.toIndex()];
    list<FieldCoords> visited;
    list<string> resultStates;

    if (figure == m_figures.cpuCrownhead || figure == m_figures.playerCrownhead)
        return list<string>();

    pair<char, char> opponentFigures = getOpponentFigures(figure);
    char opponentPawn, opponentCrownhead;
    opponentPawn = opponentFigures.first;
    opponentCrownhead = opponentFigures.second;
    FieldCoords neighs;
    bool nextRound = true;

    while (nextRound)
    {
        nextRound = false;

        for (neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y += 2)
        {
            for (neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
            {
                string childState = gameState;

                if (neighs.isOnBoard())
                {
                    if (!isCoordsVisited(visited, neighs))
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
                                    nextRound = true;

                                    // Handle multi captures with recursion
                                    pair<string, long> bestState = staticEval(childState);
                                    list<string> nextChildStates = pawnCapture(childState,
                                                                               captureCoords,
                                                                               maximizingPlayer);
                                    bestState = getNewBestState(nextChildStates, bestState, maximizingPlayer);
                                    visited.emplace_back(neighs);
                                    resultStates.emplace_back(bestState.first);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return (resultStates.size() > 0) ? resultStates : list<string>();
}

std::list<std::pair<std::string,
                    MiniMaxCheckers::FieldCoords>>
MiniMaxCheckers::findNewCrownCaptures(const std::string &gameState,
                                      const FieldCoords &direction,
                                      const FieldCoords &startCoords)
{
    bool capture = false;
    int mult = 2;
    char figure = gameState[startCoords.toIndex()];
    pair<char, char> opponents = getOpponentFigures(figure);
    list<pair<string, FieldCoords>> result;
    FieldCoords firstOpponent;
    FieldCoords current(startCoords.y + direction.y,
                        startCoords.x + direction.x);

    while (current.isOnBoard())
    {
        FieldCoords next(startCoords.y + (mult * direction.y),
                         startCoords.x + (mult * direction.x));

        if ((gameState[current.toIndex()] == opponents.first ||
             gameState[current.toIndex()] == opponents.second) &&
            capture == false)
        {
            capture = true;
            firstOpponent = current;
        }
        else if (capture == true)
        {
            if (!isFieldEmpty(gameState, current))
                break;
            else
            {
                string newState = gameState;
                newState[firstOpponent.toIndex()] = m_figures.emptyField;
                newState[startCoords.toIndex()] = m_figures.emptyField;
                newState[current.toIndex()] = figure;
                result.emplace_back(pair<string, FieldCoords>(newState, current));
            }
        }

        current = next;
        mult++;
    }

    return result;
}

std::set<std::string> MiniMaxCheckers::crownheadCapture(const std::string &gameState,
                                                        const FieldCoords &coords,
                                                        bool maximizingPlayer)
{
    set<string> resultStates;
    FieldCoords neighs;

    for (neighs.y = coords.y - 1; neighs.y <= coords.y + 1; neighs.y += 2)
    {
        for (neighs.x = coords.x - 1; neighs.x <= coords.x + 1; neighs.x += 2)
        {
            FieldCoords direction(neighs.y - coords.y, neighs.x - coords.x);
            auto captureStates = findNewCrownCaptures(gameState, direction, coords);

            for (const auto &statePair : captureStates)
            {
                auto nextState = statePair.first;
                auto nextCoords = statePair.second;
                set<string> temp = crownheadCapture(nextState,
                                                    nextCoords,
                                                    maximizingPlayer);

                for (const string &t : temp)
                    resultStates.insert(t);
            }
        }
    }

    return (resultStates.size() > 0) ? resultStates : set<string>({gameState});
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
            resultStates.emplace_back(childState);
        }
    }

    return resultStates;
}

bool MiniMaxCheckers::isCoordsVisited(const std::list<FieldCoords> &list, const FieldCoords &coords)
{
    return find_if(list.begin(), list.end(), [&coords](const auto &current) {
               return current.y == coords.y && current.x == coords.x;
           }) != list.end();
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