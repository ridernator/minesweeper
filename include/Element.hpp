#pragma once

#include <cstdint>

class Element{
    public:
        Element();

        bool isHidden();
        bool containsBomb();
        bool isLive();
        uint8_t numAdjacentBombs();

        void reveal();
        void setBomb();
        void defuse();
        void setNumAdjacentBombs(const uint8_t numAdjacentBombs);
        
    private:
        bool hidden;
        uint8_t bombs;
        bool contains;
        bool defused;
};