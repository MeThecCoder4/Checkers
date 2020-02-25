#ifndef GAME_H
#define GAME_H

#include "minimaxcheckers.h"
#include <SFML/Graphics.hpp>
#include "board.h"

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

    bool m_whitePlayer;

    sf::RenderWindow *m_window;

    std::string m_gameState;
};
}; // namespace Checkers

#endif