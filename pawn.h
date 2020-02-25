#ifndef PAWN_H
#define PAWN_H

#include "figure.h"

namespace Checkers
{
class Pawn : public Figure
{
public:
    Pawn() = default;

    Pawn(const sf::Vector2f &position, const sf::Color &color, const float radius);

    void move() override;

    void jump() override;

    virtual ~Pawn() {}
};
} // namespace Checkers

#endif