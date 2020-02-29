#ifndef FIGURE_H
#define FIGURE_H

#include <SFML/Graphics.hpp>
#include <list>

namespace Checkers
{
class Figure
{
public:
    sf::CircleShape getShape();

    void setPosition(const sf::Vector2f &position);

    virtual Figure *move(std::string &gameState,
                         const sf::Vector2u &destFieldCoords,
                         const std::vector<Figure *> &figures) = 0;

    virtual sf::Vector2u jump(std::string &gameState,
                              const sf::Vector2u &destFieldCoords,
                              const std::vector<Figure *> &figures) = 0;

    virtual ~Figure(){};

    sf::Vector2f getCoords() const;

    sf::Vector2u getBoardCoords() const;

    void setBoardCoords(const sf::Vector2u &coords);

    char getBoardSymbol();

    bool isSelected();

    void select();

    void unselect();

protected:
    Figure() = default;

    explicit Figure(const sf::Vector2u &boardCoords, const char boardSymbol);

    virtual std::list<std::pair<sf::Vector2u, std::string>> getPossibleJumps(const std::string &gameState,
                                                                             const sf::Vector2u pawnCoords) = 0;

    sf::Color m_color;

    sf::CircleShape m_shape;

    sf::Vector2u m_boardCoords;

    char m_boardSymbol;

    bool m_selected;
};
} // namespace Checkers

#endif