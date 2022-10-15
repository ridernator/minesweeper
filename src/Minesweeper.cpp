#include "Minesweeper.hpp"

#include <ncurses.h>
#include <cstdlib>
#include <string>

#define NORMAL      1
#define CURSOR      2
#define MINE        3
#define MINE_CURSOR 4

Minesweeper::Minesweeper(const uint32_t width,
                         const uint32_t height,
                         const uint32_t numMines) : width(width),
                                                    height(height),
                                                    numMines(numMines) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    int xOffset = 1;
    int yOffset = 3;
    
    init_pair(NORMAL, COLOR_GREEN, COLOR_BLACK);
    init_pair(CURSOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(MINE, COLOR_RED, COLOR_BLACK);
    init_pair(MINE_CURSOR, COLOR_RED, COLOR_GREEN);
    
    grid = new Element**[width];

    for (size_t row = 0; row < width; ++row) {
        grid[row] = new Element*[height];

        for (size_t column = 0; column < height; ++column) {
            grid[row][column] = new Element();
        }
    }

    // Insert bombs
    srand(time(NULL));
    for (uint32_t index = 0; index < numMines; ++index) {
        uint32_t x;
        uint32_t y;

        do {
            x = rand() % width;
            y = rand() % height;
        } while (grid[x][y]->containsBomb());

        grid[x][y]->setBomb();
    }

    // Calculate adjacents
    uint8_t bombCount;
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            if (!grid[x][y]->containsBomb()) {
                bombCount = 0;

                if (x != 0) {
                    bombCount += grid[x - 1][y]->containsBomb() ? 1 : 0;
                    
                    if (y != 0) {
                        bombCount += grid[x - 1][y - 1]->containsBomb() ? 1 : 0;
                    }

                    if (y != height - 1) {
                        bombCount += grid[x - 1][y + 1]->containsBomb() ? 1 : 0;
                    }
                }

                if (x != width - 1) {
                    bombCount += grid[x + 1][y]->containsBomb() ? 1 : 0;
                    
                    if (y != 0) {
                        bombCount += grid[x + 1][y - 1]->containsBomb() ? 1 : 0;
                    }

                    if (y != height - 1) {
                        bombCount += grid[x + 1][y + 1]->containsBomb() ? 1 : 0;
                    }
                }

                if (y != 0) {
                    bombCount += grid[x][y - 1]->containsBomb() ? 1 : 0;
                }

                if (y != height - 1) {
                    bombCount += grid[x][y + 1]->containsBomb() ? 1 : 0;
                }

                grid[x][y]->setNumAdjacentBombs(bombCount);
            }
        }
    }

    std::pair<uint32_t, uint32_t> cursorPos = {0, 0};
    bool exit = false;
    while (!exit) {
        clear();
        drawBox(0, 0, width + 1, 2);
        drawBox(0, 2, width + 1, height + 1);
        mvaddch(2, 0, ACS_LTEE);
        mvaddch(2, width + 1, ACS_RTEE);
        mvaddstr(1, 1, "Bombs left : ");
        
        uint32_t bombsLeft = 0;
        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                if ((grid[x][y]->containsBomb()) && (grid[x][y]->isLive())) {
                    ++bombsLeft;
                }
            }
        }
        mvaddstr(1, 14, std::to_string(bombsLeft).c_str());

        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                int colour = NORMAL;
                
                if ((x == cursorPos.first) && (y == cursorPos.second)) {
                    if ((grid[x][y]->containsBomb()) && (!grid[x][y]->isLive())) {
                        colour = MINE_CURSOR;
                    } else {
                        colour = CURSOR;
                    }
                } else if ((grid[x][y]->containsBomb()) && (!grid[x][y]->isLive())) {
                    colour = MINE;
                }

                attron(COLOR_PAIR(colour));
                if (!grid[x][y]->isHidden()) {
                    if ((grid[x][y]->containsBomb()) && (!grid[x][y]->isLive())) {
                        mvaddch(y + yOffset, x + xOffset, ACS_CKBOARD);
                    } else {
                        mvaddstr(y + yOffset, x + xOffset, std::to_string(grid[x][y]->numAdjacentBombs()).c_str());
                    }
                } else {
                    mvaddch(y + yOffset, x + xOffset, ACS_CKBOARD);
                }
                attroff(COLOR_PAIR(colour));
            }
        }

        refresh();

        switch (getch()) {
            case KEY_DOWN: {
                if (++cursorPos.second == height) {
                    cursorPos.second = 0;
                }

                break;
            }

            case KEY_UP: {
                if (cursorPos.second-- == 0) {
                    cursorPos.second = height - 1;
                }

                break;
            }

            case KEY_LEFT: {
                if (cursorPos.first-- == 0) {
                    cursorPos.first = width - 1;
                }

                break;
            }

            case KEY_RIGHT: {
                if (++cursorPos.first == width) {
                    cursorPos.first = 0;
                }

                break;
            }

            case 'q': {
                if (grid[cursorPos.first][cursorPos.second]->containsBomb()) {
                    grid[cursorPos.first][cursorPos.second]->reveal();
                    grid[cursorPos.first][cursorPos.second]->defuse();
                } else {
                    exit = true;
                }

                break;
            }

            case 'w': {
                if (grid[cursorPos.first][cursorPos.second]->containsBomb()) {
                    exit = true;
                } else if (grid[cursorPos.first][cursorPos.second]->isHidden()) {
                    reveal(cursorPos.first, cursorPos.second);
                }

                break;
            }

            case 27: {
                exit = true;

                break;
            }
        }
    }
    
    endwin();
}

void Minesweeper::reveal(const uint32_t x,
                         const uint32_t y) {
    if (grid[x][y]->isHidden()) {
        grid[x][y]->reveal();

        if (grid[x][y]->numAdjacentBombs() == 0) {
            if (x != 0) {
                reveal(x - 1, y);
                
                if (y != 0) {
                    reveal(x - 1, y - 1);
                }

                if (y != height - 1) {
                    reveal(x - 1, y + 1);
                }
            }

            if (x != width - 1) {
                reveal(x + 1, y);
                
                if (y != 0) {
                    reveal(x + 1, y - 1);
                }

                if (y != height - 1) {
                    reveal(x + 1, y + 1);
                }
            }

            if (y != 0) {
                reveal(x, y - 1);
            }

            if (y != height - 1) {
                reveal(x, y + 1);
            }
        }
    }
}

void Minesweeper::drawBox(const uint32_t x,
                          const uint32_t y,
                          const uint32_t width,
                          const uint32_t height) {
    mvhline(y, x, 0, width);
    mvhline(y + height, x, 0, width);
    mvvline(y, x, 0, height);
    mvvline(y, x + width, 0, height);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + height, x, ACS_LLCORNER);
    mvaddch(y, x + width, ACS_URCORNER);
    mvaddch(y + height, x + width, ACS_LRCORNER);
}

int main(const int argc,
         const char** argv) {
    Minesweeper minesweeper(atoi(*(argv + 1)), atoi(*(argv + 2)), atoi(*(argv + 3)));        
}