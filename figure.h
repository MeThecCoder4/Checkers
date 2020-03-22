#ifndef FIGURE_H
#define FIGURE_H

#include <SFML/Graphics.hpp>
#include <list>

namespace Checkers
{
class Figure
{
public:
    Figure() = default;

    explicit Figure(const sf::Vector2f &position, const sf::Vector2u &boardCoords,
                    const sf::Color &color, const float radius, const char boardSymbol, bool hasCrown = false);

    sf::CircleShape getShape();

    void setPosition(const sf::Vector2f &position);

    Figure *move(std::string &gameState,
                 const sf::Vector2u &destFieldCoords,
                 const std::vector<Figure *> &figures);

    bool jump(const sf::Vector2u &destFieldCoords);

    virtual ~Figure(){};

    sf::Vector2f getCoords() const;

    sf::Vector2u getBoardCoords() const;

    void setBoardCoords(const sf::Vector2u &coords);

    char getBoardSymbol();

    bool isSelected();

    void select();

    void unselect();

private:
    bool isMoveValid(const sf::Vector2u &fieldCoords,
                     const std::vector<Figure *> &figures);

    std::string isJumpValid(const sf::Vector2u &jumpCoords);

    // This method computes valid capture states using minimax algorithm.
    // It is used each time an user wants to jump, to check if that jump is indeed valid.
    std::list<std::string> getValidJumps();

    // int getEnemyCount(const std::string& gameState);

    sf::Color m_color;

    sf::CircleShape m_shape;

    sf::Vector2u m_boardCoords;

    char m_boardSymbol;

    bool m_selected;

    bool m_hasCrown;
};
} // namespace Checkers

#endif