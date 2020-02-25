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

protected:
    Figure() = default;

    sf::Color m_color;

    sf::CircleShape m_shape;
};
} // namespace Checkers

#endif