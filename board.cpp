#include "board.h"

using namespace sf;
using namespace Checkers;

Board::Board(const sf::Vector2u &windowSize)
{
    m_fieldEdgeLength = windowSize.x / m_boardSize;
    buildBoard();
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}

void Board::buildBoard()
{
    m_vertices.setPrimitiveType(Quads);
    m_vertices.resize(m_boardSize * m_boardSize * 4); // 256 vertices
                                                      // The drawing's gonna be super fast

    for (uint8_t y = 0; y < m_boardSize; y++)
    {
        for (uint8_t x = 0; x < m_boardSize; x++)
        {
            Color fieldColor = Color::White;

            if ((y % 2 == 0 && x % 2 != 0) || (y % 2 != 0 && x % 2 == 0))
                fieldColor = Color::Black;

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

float Board::getFieldEdgeLength()
{
    return m_fieldEdgeLength;
}