#include "game.h"
#include "new"

using namespace sf;
using namespace Checkers;

Game::Game(bool whitePlayer, const sf::Vector2u &windowSize)
    : m_whitePlayer(whitePlayer)
{
    if (windowSize.x != windowSize.y)
        throw "Window shape has to be a square!";

    if (!initGameWindow(windowSize))
        throw "Problem initializing game window - could not allocate memory.";

    m_gameState = "02020202"
                  "20202020"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "01010101"
                  "10101010";

    mainLoop();
}

Game::~Game()
{
    delete m_window;
}

bool Game::initGameWindow(const sf::Vector2u &windowSize)
{
    try
    {
        m_window = new RenderWindow(VideoMode(windowSize.x, windowSize.y),
                                    "Checkers");
    }
    catch (const std::bad_alloc &e)
    {
        return false;
    }

    return true;
}

void Game::mainLoop()
{
    Board m_board(m_window->getSize());

    while (m_window->isOpen())
    {
        Event event;

        while (m_window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                m_window->close();
        }

        m_window->clear(Color::Black);
        m_window->draw(m_board);
        m_window->display();
    }
}