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

    bool jump(const sf::Vector2u &destFieldCoords) override;

    ~Pawn() {}

    void runAllTests();

private:
    bool isMoveValid(const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);

    std::string isJumpValid(const sf::Vector2u &jumpCoords);

    // This method computes valid capture states using minimax algorithm.
    // It is used each time an user wants to jump, to check if that jump is indeed valid.
    std::list<std::string> getValidJumps();

};
} // namespace Checkers

#endif