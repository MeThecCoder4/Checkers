#include "pawn.h"
#include "board.h"
#include <iostream>

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

Figure *Pawn::jump(std::string &gameState,
                const sf::Vector2u &destFieldCoords,
                const std::vector<Figure *> &figures)
{
    return nullptr;
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

bool Pawn::isJumpValid(const sf::Vector2u &fieldCoords, const std::vector<Figure *> &figures)
{
    return true;
}