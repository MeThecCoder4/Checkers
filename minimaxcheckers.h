#ifndef MINIMAXCHECKERS_H
#define MINIMAXCHECKERS_H

#include "minimax.h"
#include <string>
#include <cassert>
#include <set>

class MiniMaxCheckers : public MiniMax<std::string>
{
public:
    explicit MiniMaxCheckers(const char emptyField,
                             const char cpuPawn,
                             const char playerPawn,
                             const char cpuCrownhead,
                             const char playerCrownhead,
                             const uint8_t crownheadFactor = 17,
                             const uint8_t pawnFactor = 15);

    // ~~~~~~~~~~~~~~~~~~~~~~ Test Methods ~~~~~~~~~~~~~~~~~~~~~~
    void testStaticEval();

    void testToIndex();

    void testIsOnBoard();

    void testIsFieldEmpty();

    void testCalcCaptureCoords();

    void testIsDiagonalTo();

    void testPawnCapture();

    void testPawnMove();

    void testBuildFieldChildren();

    void testBuildChildren();

    void runAllTests();

    void printState(const std::string &gameState);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::list<std::string> getCaptureStates();

    static const uint8_t m_boardEdge = 8;

protected:
    std::pair<std::string, long> staticEval(std::string gameState) override;

    std::list<std::string> buildChildren(std::string gameState, bool maximizingPlayer) override;

private:
    struct Figures
    {
        char emptyField;
        char cpuPawn;
        char playerPawn;
        char cpuCrownhead;
        char playerCrownhead;
    };

    struct FieldCoords
    {
        int8_t y;
        int8_t x;

        FieldCoords() = default;

        FieldCoords(int8_t y, int8_t x)
        {
            this->y = y;
            this->x = x;
        }

        unsigned int toIndex() const
        {
            if (y < 0 || x < 0 || y > m_boardEdge - 1 || x > m_boardEdge - 1)
                throw "FieldCoords::toIndex(): out of array range";

            return y * m_boardEdge + x;
        }

        bool isOnBoard() const
        {
            return y >= 0 && y < m_boardEdge &&
                   x >= 0 && x < m_boardEdge;
        }

        bool isDiagonalTo(const FieldCoords &otherCoords) const
        {
            return otherCoords.isOnBoard() &&
                   y != otherCoords.y &&
                   x != otherCoords.x;
        }

        bool operator==(const FieldCoords &other)
        {
            return y == other.y && x == other.x;
        }

        FieldCoords operator*(int8_t multiplier)
        {
            FieldCoords newCoords = {y, x};
            newCoords.y *= multiplier;
            newCoords.x *= multiplier;
            return newCoords;
        }

        FieldCoords &operator=(const FieldCoords &other)
        {
            if (&other == this)
                return *this;

            y = other.y;
            x = other.x;
            return *this;
        }
    };

    long pointsForFigures(const std::string& gameState);

    std::pair<std::list<std::string>, bool> buildFieldChildren(const std::string &gameState,
                                                               const FieldCoords &coords,
                                                               bool maximizingPlayer);

    std::list<std::string> crownheadCapture(const std::string &gameState,
                                       const FieldCoords &coords,
                                       bool maximizingPlayer);

    std::list<std::string> pawnCapture(const std::string &gameState,
                                       const FieldCoords &coords,
                                       bool maximizingPlayer);

    std::list<std::string> pawnMove(const std::string &gameState,
                                    const FieldCoords &coords);

    std::list<std::string> crownheadMove(const std::string &gameState,
                                         const FieldCoords &coords);

    bool isFieldEmpty(const std::string &gameState, const FieldCoords &coords);

    // This function calculates coords of a field you move your figure to after the capture
    FieldCoords calcCaptureCoords(const FieldCoords &myCoords, const FieldCoords &opponentCoords);

    void makeCrownheads(std::string &gameState);

    void updateCaptureState(std::string &gameState, const FieldCoords &opponentCoords,
                            const FieldCoords &myCurrentCoords,
                            const FieldCoords &myNextCoords,
                            char myFigure);

    std::pair<char, char> getOpponentFigures(char figure);

    std::pair<std::string, long> getNewBestState(const std::list<std::string> &nextStates,
                                                 std::pair<std::string, long> currentBest,
                                                 bool maximizingPlayer);

    std::list<std::string> m_captureStates;

    uint8_t m_crownheadFactor;

    uint8_t m_pawnFactor;

    Figures m_figures;
};

#endif