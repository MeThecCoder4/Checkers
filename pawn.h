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
         const sf::Color &color, const float radius, const char boardSymbol);

    Figure *move(std::string &gameState,
                 const sf::Vector2u &destFieldCoords,
                 const std::vector<Figure *> &figures) override;

    sf::Vector2u jump(std::string &gameState,
                 const sf::Vector2u &destFieldCoords,
                 const std::vector<Figure *> &figures) override;

    ~Pawn() {}

private:
    bool isMoveValid(const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);

    bool isJumpValid(const std::string &gameState,
                     const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);
};
} // namespace Checkers

#endif