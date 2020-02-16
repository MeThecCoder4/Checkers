#ifndef MINIMAXCHECKERS_H
#define MINIMAXCHECKERS_H

#include "minimax.h"
#include <string>
#include <cassert>

class MiniMaxCheckers : public MiniMax<std::string>
{
public:
    explicit MiniMaxCheckers(const char emptyField,
                             const char cpuPawn,
                             const char playerPawn,
                             const char cpuCrownhead,
                             const char playerCrownhead,
                             const uint8_t crownheadFactor = 2, 
                             const uint8_t pawnFactor = 1);

    // Test methods
    void testStaticEval();

    void testToIndex();

    void testIsOnBoard();

    void testIsFieldEmpty();

    void testCalcCaptureCoords();

    void testIsDiagonalTo();

    void testPawnCapture();

    void printState(const std::string& gameState);

protected:
    std::pair<std::string, int> staticEval(std::string gameState) override;

    std::list<std::string> buildChildren(std::string gameState) override;

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
        uint8_t y;
        uint8_t x;

        unsigned int toIndex() const
        {
            if(y > m_boardEdge - 1 || x > m_boardEdge - 1)
                throw "FieldCoords::toIndex(): out of array range";

            return y * m_boardEdge + x;
        }

        bool isOnBoard() const
        {
            return y < m_boardEdge &&
                   x < m_boardEdge;
        }

        bool isDiagonalTo(const FieldCoords& otherCoords) const
        {
            return otherCoords.isOnBoard() &&
                   y != otherCoords.y && 
                   x != otherCoords.x;
        }

        bool operator==(const FieldCoords& other)
        {
            return y == other.y && x == other.x;
        }
    };

    std::list<std::string> buildFieldChildren(const std::string& gameState,
                                              const FieldCoords& coords);

    std::string pawnCapture(const std::string& gameState,
                            const FieldCoords& coords,
                            const std::list<FieldCoords>& visited);

    bool isFieldEmpty(const std::string& gameState, const FieldCoords& coords);                            

    // This function calculates coords of a field you move your figure to after the capture
    FieldCoords calcCaptureCoords(const FieldCoords& myCoords, const FieldCoords& opponentCoords);

    static const uint8_t m_boardEdge = 8;

    uint8_t m_crownheadFactor;

    uint8_t m_pawnFactor;

    Figures m_figures;
};

#endif