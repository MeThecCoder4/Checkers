#include "pawn.h"
#include "board.h"

using namespace sf;
using namespace Checkers;

Pawn::Pawn(const sf::Vector2f &position, const sf::Vector2u &boardCoords,
           const sf::Color &color, const float radius)
    : Figure(boardCoords)
{
    m_shape.setOrigin(radius, radius);
    m_shape.setFillColor(color);
    m_shape.setPointCount(36);
    m_shape.setScale(1.0f, 1.0f);
    m_shape.setRadius(radius);
    m_shape.setPosition(position);
}

void Pawn::move()
{
}

void Pawn::jump()
{
}

bool Pawn::isMoveValid(const sf::Vector2u &fieldCoords, const std::vector<Figure *> &figures)
{
    if (Board::isFieldValid(fieldCoords) && Board::isFieldEmpty(figures, fieldCoords))
    {
        
    }

    return true;
}

bool Pawn::isJumpValid(const sf::Vector2u &fieldCoords, const std::vector<Figure *> &figures)
{
    return true;
}