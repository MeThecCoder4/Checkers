#include "pawn.h"
#include "board.h"
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;
using namespace sf;
using namespace Checkers;

Pawn::Pawn(const sf::Vector2f &position, const sf::Vector2u &boardCoords,
           const sf::Color &color, const float radius, const char boardSymbol)
    : Figure(boardCoords, boardSymbol)
{
    m_shape.setOrigin(radius, radius);
    m_shape.setFillColor(color);
    m_shape.setPointCount(36);
    m_shape.setScale(1.0f, 1.0f);
    m_shape.setRadius(radius);
    m_shape.setPosition(position);
}

Figure *Pawn::move(std::string &gameState,
                   const sf::Vector2u &destFieldCoords,
                   const std::vector<Figure *> &figures)
{
    if (isMoveValid(destFieldCoords, figures) && isSelected())
    {
        gameState[destFieldCoords.y * Board::getBoardSize() + destFieldCoords.x] = m_boardSymbol;
        gameState[m_boardCoords.y * Board::getBoardSize() + m_boardCoords.x] = Board::Symbols::EmptyField;
        m_boardCoords = destFieldCoords;
        unselect();
        return this;
    }

    return nullptr;
}

sf::Vector2u Pawn::jump(std::string &gameState,
                        const sf::Vector2u &destFieldCoords,
                        const std::vector<Figure *> &figures)
{
    // if (isJumpValid(gameState, destFieldCoords, figures) && isSelected())
    // {
    //     Vector2i direction(destFieldCoords.x - m_boardCoords.x, destFieldCoords.y - m_boardCoords.y);
    //     Vector2u opponentCoords(m_boardCoords.x + (direction.x / 2),
    //                             m_boardCoords.y + (direction.y / 2));

    //     gameState[destFieldCoords.y * Board::getBoardSize() + destFieldCoords.x] = m_boardSymbol;
    //     gameState[m_boardCoords.y * Board::getBoardSize() + m_boardCoords.x] = Board::Symbols::EmptyField;
    //     gameState[opponentCoords.y * Board::getBoardSize() + opponentCoords.x] = Board::Symbols::EmptyField;
    //     unselect();
    //     return opponentCoords;
    // }

    return Vector2u(8, 8);
}

bool Pawn::isMoveValid(const sf::Vector2u &fieldCoords,
                       const std::vector<Figure *> &figures)
{
    if (Board::isFieldValid(fieldCoords) && Board::isFieldEmpty(figures, fieldCoords))
    {
        Vector2i direction(fieldCoords.x - m_boardCoords.x, fieldCoords.y - m_boardCoords.y);

        if (abs(direction.x) == 1 && abs(direction.y) == 1)
        {
            if (direction.y < 0)
            {
                return true;
            }
        }
    }

    return false;
}

std::list<std::pair<sf::Vector2u, std::string>> Pawn::getPossibleJumps(const std::string &gameState, const sf::Vector2u pawnCoords)
{
    if (isSelected())
    {
        list<std::pair<sf::Vector2u, std::string>> resultStates;
        uint8_t globalBest = 0;
        list<Vector2u> visited;
        bool nextRound = true;

        while (nextRound)
        {
            nextRound = false;
            Vector2u neighCoords;

            for (neighCoords.y = pawnCoords.y - 1; neighCoords.y <= pawnCoords.y + 1; neighCoords.y += 2)
            {
                for (neighCoords.x = pawnCoords.x - 1; neighCoords.x <= pawnCoords.x + 1; neighCoords.x += 2)
                {
                    string childState = gameState;

                    if (Board::isOnBoard(neighCoords))
                    {
                        // If wasn't visited before
                        if (find(visited.begin(), visited.end(), neighCoords) == visited.end())
                        {
                            if (childState[Board::toIndex(neighCoords)] == Board::Symbols::OpponentPawn ||
                                childState[Board::toIndex(neighCoords)] == Board::Symbols::OpponentCrownhead)
                            {
                                Vector2i capDir(neighCoords - pawnCoords);
                                Vector2u capCoords(pawnCoords.x + (2 * capDir.x),
                                                   pawnCoords.y + (2 * capDir.y));

                                if (Board::isFieldEmpty(childState, capCoords))
                                {
                                    childState[Board::toIndex(capCoords)] = Board::Symbols::MyPawn;
                                    childState[Board::toIndex(neighCoords)] = Board::Symbols::EmptyField;
                                    childState[Board::toIndex(pawnCoords)] = Board::Symbols::EmptyField;
                                    nextRound = true;

                                    // Handle multiple captures with recursion
                                    pair<Vector2u, string> best(capCoords, childState);
                                    uint8_t bestEval = Board::emptyFieldsNo(childState);
                                    auto childStates = getPossibleJumps(childState, capCoords);

                                    for (const auto &state : childStates)
                                    {
                                        uint8_t nextEval = 0;

                                        if ((nextEval = Board::emptyFieldsNo(state.second)) > bestEval)
                                        {
                                            best = state;
                                            bestEval = nextEval;
                                            
                                            if(bestEval > globalBest)
                                                globalBest = bestEval;
                                        }
                                    }

                                    visited.emplace_back(neighCoords);
                                    resultStates.emplace_back(best);
                                }
                            }
                        }
                    }
                }
            }
        }

        list<pair<Vector2u, std::string>> bestStates;

        for(const auto& state : resultStates)
            if(Board::emptyFieldsNo(state.second) >= globalBest)
                bestStates.emplace_back(state);

        return (bestStates.size() > 0) ? bestStates : list<pair<Vector2u, std::string>>();
    }

    return list<pair<Vector2u, std::string>>();
}