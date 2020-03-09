#include "figure.h"
#include "board.h"
#include "minimaxcheckers.h"
#include <iostream>

using namespace std;
using namespace sf;
using namespace Checkers;

Figure::Figure(const sf::Vector2f &position, const sf::Vector2u &boardCoords,
               const sf::Color &color, const float radius, const char boardSymbol, bool hasCrown)
    : m_boardCoords(boardCoords), m_boardSymbol(boardSymbol), m_hasCrown(hasCrown)
{
    m_shape.setOrigin(radius, radius);
    m_shape.setFillColor(color);

    if (hasCrown)
        m_shape.setPointCount(8);
    else
        m_shape.setPointCount(36);

    m_shape.setScale(1.0f, 1.0f);
    m_shape.setRadius(radius);
    m_shape.setPosition(position);
}

Figure *Figure::move(std::string &gameState,
                     const sf::Vector2u &destFieldCoords,
                     const std::vector<Figure *> &figures)
{
    if (isMoveValid(destFieldCoords, figures) && isSelected())
    {
        gameState[destFieldCoords.y * Board::getBoardSize() + destFieldCoords.x] = m_boardSymbol;
        gameState[m_boardCoords.y * Board::getBoardSize() + m_boardCoords.x] = Board::Symbols::EmptyField;
        m_boardCoords = destFieldCoords;
        unselect();
        return this;
    }

    return nullptr;
}

bool Figure::jump(const sf::Vector2u &destFieldCoords)
{
    string afterJumpState;

    if ((afterJumpState = isJumpValid(destFieldCoords)) != string() && isSelected())
    {
        Board::gameState = afterJumpState;
        unselect();
        return true;
    }

    return false;
}

bool Figure::isMoveValid(const sf::Vector2u &fieldCoords,
                         const std::vector<Figure *> &figures)
{
    if (Board::isFieldValid(fieldCoords) && Board::isFieldEmpty(figures, fieldCoords))
    {
        Vector2i direction(fieldCoords.x - m_boardCoords.x, fieldCoords.y - m_boardCoords.y);

        if (abs(direction.x) == 1 && abs(direction.y) == 1)
        {
            if (direction.y < 0 || m_hasCrown)
            {
                return true;
            }
        }
    }

    return false;
}

// This method works by computing all valid capture states
// and checking if any of those have gameState[jumpCoords] set to
// player's pawn or crownhead.
std::string Figure::isJumpValid(const sf::Vector2u &jumpCoords)
{
    list<string> validStates = getValidJumps();

    for (string state : validStates)
    {
        reverse(state.begin(), state.end());
        cout << "Here" << endl;

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                cout << state[i * 8 + j];
            }

            cout << endl;
        }

        if (state[Board::toIndex(jumpCoords)] == Board::Symbols::MyPawn ||
            state[Board::toIndex(jumpCoords)] == Board::Symbols::MyCrownhead)
            return state;
    }

    return string();
}

std::list<std::string> Figure::getValidJumps()
{
    MiniMaxCheckers mmc(Board::Symbols::EmptyField,
                        Board::Symbols::OpponentPawn,
                        Board::Symbols::MyPawn,
                        Board::Symbols::OpponentCrownhead,
                        Board::Symbols::MyCrownhead);
    string revGameState = Board::gameState;
    reverse(revGameState.begin(), revGameState.end());
    // false means user move
    mmc.search(revGameState, 1, LONG_MIN, LONG_MAX, false);
    list<string> validStates = mmc.getCaptureStates();
    return validStates;
}

CircleShape Figure::getShape()
{
    return m_shape;
}

sf::Vector2f Figure::getCoords() const
{
    return m_shape.getPosition();
}

sf::Vector2u Figure::getBoardCoords() const
{
    return m_boardCoords;
}

void Figure::setBoardCoords(const sf::Vector2u &coords)
{
    m_boardCoords = coords;
}

void Figure::setPosition(const sf::Vector2f &position)
{
    m_shape.setPosition(position);
}

bool Figure::isSelected()
{
    return m_selected;
}

void Figure::select()
{
    m_selected = true;
}

void Figure::unselect()
{
    m_selected = false;
}

char Figure::getBoardSymbol()
{
    return m_boardSymbol;
}