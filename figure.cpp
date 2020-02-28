#include "figure.h"

using namespace sf;
using namespace Checkers;

Figure::Figure(const sf::Vector2u &boardCoords)
    : m_boardCoords(boardCoords)
{
}

CircleShape Figure::getShape()
{
    return m_shape;
}

sf::Vector2f Figure::getCoords() const
{
    return m_shape.getPosition();
}

sf::Vector2u Figure::getBoardCoords() const
{
}

void Figure::setBoardCoords(const sf::Vector2u &coords)
{
}

void Figure::setPosition(const sf::Vector2f &position)
{
}