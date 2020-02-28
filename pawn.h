#ifndef PAWN_H
#define PAWN_H

#include "figure.h"
#include <vector>

namespace Checkers
{
class Pawn : public Figure
{
public:
    Pawn() = default;

    Pawn(const sf::Vector2f &position, const sf::Vector2u &boardCoords,
         const sf::Color &color, const float radius);

    void move() override;

    void jump() override;

    ~Pawn() {}

private:
    bool isMoveValid(const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);

    bool isJumpValid(const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);
};
} // namespace Checkers

#endif