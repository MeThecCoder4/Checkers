#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>

namespace Checkers
{
class Board : public sf::Drawable, public sf::Transformable
{
public:
    Board() = default;

    explicit Board(const sf::Vector2u& windowSize);

    virtual ~Board() = default;

    float getFieldEdgeLength();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void buildBoard();

    sf::VertexArray m_vertices;

    float m_fieldEdgeLength;

    const uint8_t m_boardSize = 8;
};
}; // namespace Checkers

#endif