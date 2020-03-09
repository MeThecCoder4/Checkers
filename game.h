#ifndef GAME_H
#define GAME_H

#include "minimaxcheckers.h"
#include <SFML/Graphics.hpp>
#include "board.h"
#include "figure.h"
#include <vector>

namespace Checkers
{
class Game
{
public:
    Game(){};

    Game(unsigned char difficulty, bool whitePlayer, const sf::Vector2u &windowSize = sf::Vector2u(600, 600));

    ~Game();

private:
    void mainLoop();

    bool initGameWindow(const sf::Vector2u &windowSize);

    void buildFiguresFrom(const std::string &gameState);

    void drawFigures();

    void mouseEvents();

    void selectOnClick(const sf::Vector2u &clickedCoords);

    bool isMovable(const sf::Vector2u &clickedCoords);

    bool removeFigureAt(const sf::Vector2u& coords);

    bool m_whitePlayer;

    void moveFigure(Figure& figure, const sf::Vector2u& destinationCoords);

    void resolveRound();

    sf::RenderWindow *m_window;

    std::vector<Figure *> m_figures;

    Board m_board;

    Figure* m_lastSelected;

    bool m_playerTurn;

    unsigned char m_difficulty;

};
}; // namespace Checkers

#endif