#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "clickablerect.h"
#include "figure.h"

namespace Checkers
{
class Board : public sf::Drawable, public sf::Transformable
{
public:
    Board() = default;

    explicit Board(const sf::Vector2u &windowSize);

    virtual ~Board() = default;

    float getFieldEdgeLength() const;

    void setFieldEdgeLength(const int windowEdgeLength);

    void build();

    sf::Vector2u getClickedCoords(const sf::Vector2i &mousePos);

    static bool isFieldValid(const sf::Vector2u &fieldCoords);

    static bool isFieldEmpty(const std::vector<Figure *> &figures,
                             const sf::Vector2u &fieldCoords);

    static uint8_t getBoardSize();

    enum Symbols
    {
        EmptyField = '0',
        OpponentPawn = '1',
        MyPawn = '2',
        OpponentCrownhead = '3',
        MyCrownhead = '4'
    };

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;

    std::vector<ClickableRect> m_fieldRects;

    float m_fieldEdgeLength;

    static const uint8_t m_boardSize = 8;
};
}; // namespace Checkers

#endif