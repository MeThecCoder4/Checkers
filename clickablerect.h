#ifndef CLICKABLE_RECT_H
#define CLICKABLE_RECT_H

#include <SFML/Graphics.hpp>

namespace Checkers
{
class ClickableRect : public sf::Rect<float>
{
public:
    ClickableRect(const sf::Vector2f &position, const sf::Vector2f &size);

    bool clicked(const sf::Vector2i &mousePos);

private:
    void checkPressedIn(const sf::Vector2i &mousePos);

    void checkReleasedIn(const sf::Vector2i &mousePos);

    bool m_pressedIn;

    bool m_releasedIn;

};
} // namespace Checkers

#endif