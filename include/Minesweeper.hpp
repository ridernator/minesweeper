#pragma once

#include "Element.hpp"

#include <cstdint>

class Minesweeper {
    public:
        Minesweeper(const uint32_t width,
                    const uint32_t height,
                    const uint32_t numMines);

    private:
        void drawBox(const uint32_t x,
                     const uint32_t y,
                     const uint32_t width,
                     const uint32_t height);

        void reveal(const uint32_t x,
                    const uint32_t y);

        Element*** grid;

        const uint32_t width;
        const uint32_t height;
        const uint32_t numMines;

};