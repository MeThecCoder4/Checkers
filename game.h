#ifndef GAME_H
#define GAME_H

#include "minimaxcheckers.h"
#include <SFML/Graphics.hpp>
#include "board.h"
#include "pawn.h"
#include <vector>

namespace Checkers
{
class Game
{
public:
    Game(){};

    Game(bool whitePlayer, const sf::Vector2u &windowSize = sf::Vector2u(600, 600));

    ~Game();

private:
    void mainLoop();

    bool initGameWindow(const sf::Vector2u &windowSize);

    void initFiguresFrom(const std::string &gameState);

    void drawFigures();

    bool m_whitePlayer;

    sf::RenderWindow *m_window;

    std::string m_gameState;

    std::vector<Figure *> m_figures;

    Board m_board;
};
}; // namespace Checkers

#endif