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

    m_board.setFieldEdgeLength(windowSize.x);
    m_board.build();

    m_gameState = "02020202"
                  "20202020"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "01010101"
                  "10101010";

    initFiguresFrom(m_gameState);
    mainLoop();
}

Game::~Game()
{
    for (const auto &figure : m_figures)
        delete figure;

    delete m_window;
}

bool Game::initGameWindow(const sf::Vector2u &windowSize)
{
    try
    {
        ContextSettings settings;
        settings.antialiasingLevel = 8;

        m_window = new RenderWindow(VideoMode(windowSize.x, windowSize.y),
                                    "Checkers", Style::Close | Style::Titlebar, settings);
    }
    catch (const std::bad_alloc &e)
    {
        return false;
    }

    return true;
}

void Game::mainLoop()
{
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
        drawFigures();
        m_window->display();
    }
}

void Game::initFiguresFrom(const std::string &gameState)
{
    if (m_figures.size() == 0)
    {
        for (uint8_t y = 0; y < m_board.getBoardSize(); y++)
        {
            for (uint8_t x = 0; x < m_board.getBoardSize(); x++)
            {
                if (Board::isFieldValid(Vector2u(x, y)))
                {
                    Vector2f position(x * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2),
                                      y * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2));

                    float figureRadius = (m_board.getFieldEdgeLength() / 2) - 5.0f;

                    switch (gameState[y * m_board.getBoardSize() + x])
                    {
                    // CPU pawn
                    case '1':
                        m_figures.emplace_back(new Pawn(position,
                                                        (m_whitePlayer ? Color(127, 51, 0) : Color::White),
                                                        figureRadius));
                        break;
                    // Player pawn
                    case '2':
                        m_figures.emplace_back(new Pawn(position,
                                                        (m_whitePlayer ? Color::White : Color(127, 51, 0)),
                                                        figureRadius));
                        break;
                    // CPU crownhead
                    case '3':
                        break;
                    // Player crownhead
                    case '4':
                        break;
                    }
                }
            }
        }
    }
}

void Game::drawFigures()
{
    for (const auto &figure : m_figures)
        m_window->draw(figure->getShape());
}