#include "figure.h"
#include <iostream>

using namespace std;
using namespace sf;
using namespace Checkers;

Figure::Figure(const sf::Vector2u &boardCoords, const char boardSymbol)
    : m_boardCoords(boardCoords), m_boardSymbol(boardSymbol)
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
    return m_boardCoords;
}

void Figure::setBoardCoords(const sf::Vector2u &coords)
{
    m_boardCoords = coords;
}

void Figure::setPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
}

bool Figure::isSelected()
{
    return m_selected;
}

void Figure::select()
{
    m_selected = true;
}

void Figure::unselect()
{
    m_selected = false;
}

char Figure::getBoardSymbol()
{
    return m_boardSymbol;
}