#include "game.h"
#include "new"
#include <iostream>

using namespace std;
using namespace sf;
using namespace Checkers;

Game::Game(bool whitePlayer, const sf::Vector2u &windowSize)
    : m_whitePlayer(whitePlayer), m_lastSelected(nullptr)
{
    if (windowSize.x != windowSize.y)
        throw "Window shape has to be a square!";

    if (!initGameWindow(windowSize))
        throw "Problem initializing game window - could not allocate memory.";

    m_board.setFieldEdgeLength(windowSize.x);
    m_board.build();

    m_gameState = "01010101"
                  "10101010"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "02020202"
                  "20202020";

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

        m_window->setFramerateLimit(30);
        m_window->setVerticalSyncEnabled(true);
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
        mouseEvents();
        drawFigures();
        m_window->display();
    }
}

void Game::initFiguresFrom(const std::string &gameState)
{
    if (m_figures.size() == 0)
    {
        for (uint8_t y = 0; y < Board::getBoardSize(); y++)
        {
            for (uint8_t x = 0; x < Board::getBoardSize(); x++)
            {
                if (Board::isFieldValid(Vector2u(x, y)))
                {
                    Vector2f position(x * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2),
                                      y * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2));

                    float figureRadius = (m_board.getFieldEdgeLength() / 2) - 5.0f;

                    switch (gameState[y * Board::getBoardSize() + x])
                    {
                    case Board::Symbols::OpponentPawn:
                        m_figures.emplace_back(new Pawn(position, Vector2u(x, y),
                                                        (m_whitePlayer ? Color::Red : Color::White),
                                                        figureRadius, Board::Symbols::OpponentPawn));
                        break;
                    case Board::Symbols::MyPawn:
                        m_figures.emplace_back(new Pawn(position, Vector2u(x, y),
                                                        (m_whitePlayer ? Color::White : Color::Red),
                                                        figureRadius, Board::Symbols::MyPawn));
                        break;
                    case Board::Symbols::OpponentCrownhead:
                        break;
                    case Board::Symbols::MyCrownhead:
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

void Game::mouseEvents()
{
    Vector2u clickedCoords = m_board.getClickedCoords(Mouse::getPosition(*m_window));

    // (8, 8) is out of board
    if (clickedCoords != Vector2u(8, 8))
    {
        if (Board::isFieldValid(clickedCoords))
        {
            // Select figure on click
            if (isMovable(clickedCoords))
            {
                selectOnClick(clickedCoords);
            }
            // If no figure was clicked - move already selected
            else
            {
                if (m_lastSelected != nullptr)
                {
                    if (m_lastSelected->move(m_gameState, clickedCoords,
                                             m_figures) != nullptr)
                    {
                        moveFigure(*m_lastSelected, clickedCoords);
                    }
                }
            }
        }

        std::cout << "Clicked coords: (" << clickedCoords.x << ", " << clickedCoords.y << ")" << std::endl;

        if (m_lastSelected != nullptr)
        {
            Vector2u lsCoords = m_lastSelected->getBoardCoords();
            std::cout << "Last selected: (" << lsCoords.x << ", " << lsCoords.y << ")" << std::endl;
        }
    }
}

void Game::selectOnClick(const Vector2u &clickedCoords)
{
    for (auto &figure : m_figures)
    {
        if (figure->getBoardCoords() == clickedCoords)
        {
            if (m_lastSelected != nullptr)
                m_lastSelected->unselect();

            figure->select();
            m_lastSelected = figure;
        }
    }
}

bool Game::isMovable(const sf::Vector2u &clickedCoords)
{
    for (const auto &figure : m_figures)
    {
        if (figure->getBoardCoords() == clickedCoords &&
            (figure->getBoardSymbol() == Board::Symbols::MyPawn ||
             figure->getBoardSymbol() == Board::Symbols::MyCrownhead))
        {
            return true;
        }
    }

    return false;
}

void Game::moveFigure(Figure &figure, const sf::Vector2u &destinationCoords)
{
    Vector2f newPosition(destinationCoords.x * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2),
                         destinationCoords.y * m_board.getFieldEdgeLength() + (m_board.getFieldEdgeLength() / 2));
    figure.setPosition(newPosition);
}