#include "pawn.h"

using namespace sf;
using namespace Checkers;

Pawn::Pawn(const sf::Vector2f &position, const sf::Color &color, const float radius)
{
    m_shape.setOrigin(radius, radius);
    m_shape.setFillColor(color);
    m_shape.setPointCount(72);
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