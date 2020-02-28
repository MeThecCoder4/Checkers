#include "pawn.h"
#include "board.h"
#include <iostream>

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
    if (isJumpValid(gameState, destFieldCoords, figures) && isSelected())
    {
        Vector2i direction(destFieldCoords.x - m_boardCoords.x, destFieldCoords.y - m_boardCoords.y);
        Vector2u opponentCoords(m_boardCoords.x + (direction.x / 2),
                                    m_boardCoords.y + (direction.y / 2));

        gameState[destFieldCoords.y * Board::getBoardSize() + destFieldCoords.x] = m_boardSymbol;
        gameState[m_boardCoords.y * Board::getBoardSize() + m_boardCoords.x] = Board::Symbols::EmptyField;
        gameState[opponentCoords.y * Board::getBoardSize() + opponentCoords.x] = Board::Symbols::EmptyField;
        unselect();
        return opponentCoords;
    }

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

bool Pawn::isJumpValid(const std::string &gameState,
                       const sf::Vector2u &fieldCoords,
                       const std::vector<Figure *> &figures)
{
    if (Board::isFieldValid(fieldCoords) && Board::isFieldEmpty(figures, fieldCoords))
    {
        Vector2i direction(fieldCoords.x - m_boardCoords.x, fieldCoords.y - m_boardCoords.y);

        if (abs(direction.x) == 2 && abs(direction.y) == 2)
        {
            Vector2u opponentCoords(m_boardCoords.x + (direction.x / 2),
                                    m_boardCoords.y + (direction.y / 2));
            
            if ((gameState[opponentCoords.y * Board::getBoardSize() + opponentCoords.x] == Board::Symbols::OpponentPawn) ||
                (gameState[opponentCoords.y * Board::getBoardSize() + opponentCoords.x] == Board::Symbols::OpponentCrownhead))
            {
                return true;
            }
        }
    }

    return false;
}