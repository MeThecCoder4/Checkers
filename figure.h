#ifndef FIGURE_H
#define FIGURE_H

#include <SFML/Graphics.hpp>

namespace Checkers
{
class Figure
{
public:
    sf::CircleShape getShape();

    virtual void move() = 0;

    virtual void jump() = 0;

    virtual ~Figure() {};

    sf::Vector2f getCoords() const;

    sf::Vector2u getBoardCoords() const;

    void setBoardCoords(const sf::Vector2u& coords);

    void setPosition(const sf::Vector2f& position);

protected:
    Figure() = default;

    explicit Figure(const sf::Vector2u& boardCoords);

    sf::Color m_color;

    sf::CircleShape m_shape;

    sf::Vector2u m_boardCoords;
};
} // namespace Checkers

#endif