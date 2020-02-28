#include "clickablerect.h"

#define MIN_TIME_BETWEEN_CLICKS_MS 400
#define MAX_CLICK_TIME 400

using namespace Checkers;
using namespace sf;

ClickableRect::ClickableRect(const sf::Vector2f &position, const sf::Vector2f &size)
    : Rect(position, size), m_pressedIn(false), m_releasedIn(false)
{
    m_lastClick = m_timePassed.getElapsedTime();
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
        if ((m_timePassed.getElapsedTime().asMilliseconds() - m_lastClick.asMilliseconds()) > MIN_TIME_BETWEEN_CLICKS_MS)
        {
            m_pressedIn = true;
            m_lastClick = m_timePassed.getElapsedTime();
        }
    }
}

void ClickableRect::checkReleasedIn(const sf::Vector2i &mousePos)
{

    if (!Mouse::isButtonPressed(Mouse::Left) &&
        contains(Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))))
    {
        if((m_timePassed.getElapsedTime().asMilliseconds() - m_lastClick.asMilliseconds()) < MAX_CLICK_TIME)
            m_releasedIn = true;
    }
}