#ifndef FIGURE_H
#define FIGURE_H

#include <SFML/Graphics.hpp>

namespace Checkers
{
class Figure
{
public:
    sf::CircleShape getShape();

    virtual Figure *move(std::string &gameState,
                         const sf::Vector2u &destFieldCoords,
                         const std::vector<Figure *> &figures) = 0;

    virtual Figure *jump(std::string &gameState,
                         const sf::Vector2u &destFieldCoords,
                         const std::vector<Figure *> &figures) = 0;

    virtual ~Figure(){};

    sf::Vector2f getCoords() const;

    sf::Vector2u getBoardCoords() const;

    void setBoardCoords(const sf::Vector2u &coords);

    void setPosition(const sf::Vector2f &position);

    char getBoardSymbol();

    bool isSelected();

    void select();

    void unselect();

protected:
    Figure() = default;

    explicit Figure(const sf::Vector2u &boardCoords, const char boardSymbol);

    sf::Color m_color;

    sf::CircleShape m_shape;

    sf::Vector2u m_boardCoords;

    char m_boardSymbol;

    bool m_selected;
};
} // namespace Checkers

#endif