#include "clickablerect.h"

using namespace Checkers;
using namespace sf;

ClickableRect::ClickableRect(const sf::Vector2f &position, const sf::Vector2f &size)
    : Rect(position, size), m_pressedIn(false), m_releasedIn(false)
{
}

bool ClickableRect::clicked(const sf::Vector2i &mousePos)
{
    checkPressedIn(mousePos);
    checkReleasedIn(mousePos);

    if (m_pressedIn == true && m_releasedIn == true)
    {
        m_pressedIn = false;
        m_releasedIn = false;
        return true;
    }

    return false;
}

void ClickableRect::checkPressedIn(const sf::Vector2i &mousePos)
{
    if (Mouse::isButtonPressed(Mouse::Left) &&
        contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))))
    {
        m_pressedIn = true;
    }
}

void ClickableRect::checkReleasedIn(const sf::Vector2i &mousePos)
{

    if (!Mouse::isButtonPressed(Mouse::Left) &&
        contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))))
    {
        m_releasedIn = true;
    }
}