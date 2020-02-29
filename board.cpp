#include "board.h"

using namespace sf;
using namespace Checkers;

std::string Board::gameState = "01010101"
                               "10000010"
                               "00000000"
                               "00001000"
                               "00000000"
                               "00100000"
                               "02020202"
                               "20202020";

Board::Board(const sf::Vector2u &windowSize)
{
    m_fieldEdgeLength = windowSize.x / m_boardSize;
    build();
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}

void Board::build()
{
    m_vertices.setPrimitiveType(Quads);
    m_vertices.resize(m_boardSize * m_boardSize * 4); // 256 vertices
                                                      // The drawing's gonna be super fast

    for (uint8_t y = 0; y < m_boardSize; y++)
    {
        for (uint8_t x = 0; x < m_boardSize; x++)
        {
            Color fieldColor = Color::White;

            if (isFieldValid(Vector2u(x, y)))
                fieldColor = Color::Black;

            m_fieldRects.emplace_back(ClickableRect(
                Vector2f(x * m_fieldEdgeLength, y * m_fieldEdgeLength),
                Vector2f(m_fieldEdgeLength, m_fieldEdgeLength)));

            // Pointer to current quad
            Vertex *quad = &m_vertices[(y * m_boardSize + x) * 4];

            quad[0].position = Vector2f(x * m_fieldEdgeLength,
                                        y * m_fieldEdgeLength);
            quad[0].color = fieldColor;

            quad[1].position = Vector2f((x + 1) * m_fieldEdgeLength,
                                        y * m_fieldEdgeLength);
            quad[1].color = fieldColor;

            quad[2].position = Vector2f((x + 1) * m_fieldEdgeLength,
                                        (y + 1) * m_fieldEdgeLength);
            quad[2].color = fieldColor;

            quad[3].position = Vector2f(x * m_fieldEdgeLength,
                                        (y + 1) * m_fieldEdgeLength);
            quad[3].color = fieldColor;
        }
    }
}

float Board::getFieldEdgeLength() const
{
    return m_fieldEdgeLength;
}

void Board::setFieldEdgeLength(const int windowEdgeLength)
{
    m_fieldEdgeLength = windowEdgeLength / m_boardSize;
}

uint8_t Board::getBoardSize()
{
    return m_boardSize;
}

bool Board::isFieldValid(const sf::Vector2u &fieldCoords)
{
    uint8_t x = fieldCoords.x;
    uint8_t y = fieldCoords.y;
    return (y % 2 == 0 && x % 2 != 0) || (y % 2 != 0 && x % 2 == 0);
}

sf::Vector2u Board::getClickedCoords(const Vector2i &mousePos)
{

    for (uint8_t y = 0; y < m_boardSize; y++)
    {
        for (uint8_t x = 0; x < m_boardSize; x++)
        {
            if (m_fieldRects[y * m_boardSize + x].clicked(mousePos))
                return Vector2u(x, y);
        }
    }

    // (8, 8) is out of board
    return Vector2u(8, 8);
}

bool Board::isFieldEmpty(const std::vector<Figure *> &figures,
                         const sf::Vector2u &fieldCoords)
{
    for (const auto &figure : figures)
    {
        if (figure->getBoardCoords() == fieldCoords)
            return false;
    }

    return true;
}

bool Board::isFieldEmpty(const std::string &gameState,
                         const sf::Vector2u &coords)
{
    if (coords.x >= 0 && coords.y >= 0 &&
        coords.x < m_boardSize && coords.y < m_boardSize)
    {
        return gameState[coords.y * m_boardSize + coords.x] == Symbols::EmptyField;
    }

    return false;
}

bool Board::isOnBoard(const sf::Vector2u &coords)
{
    return coords.x >= 0 && coords.y >= 0 &&
           coords.x < m_boardSize && coords.y < m_boardSize;
}

uint8_t Board::toIndex(const sf::Vector2u& coords)
{
    return coords.y * m_boardSize + coords.x;
}

uint8_t Board::emptyFieldsNo(const std::string& gameState)
{
    uint8_t counter = 0;
    Vector2u coords;

    for(coords.y = 0; coords.y < m_boardSize; coords.y++)
    {
        for(coords.x = 0; coords.x < m_boardSize; coords.x++)
        {
            if(isFieldValid(coords) && isFieldEmpty(gameState, coords))
                counter++;
        }
    }

    return counter;
}