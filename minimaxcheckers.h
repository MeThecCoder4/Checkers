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

    // Test function
    void testStaticEval();

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

    std::string buildChild(const std::string& gameState);

    const uint8_t m_boardEdge = 8;

    uint8_t m_crownheadFactor = 2;

    uint8_t m_pawnFactor = 1;

    Figures m_figures;
};

#endif