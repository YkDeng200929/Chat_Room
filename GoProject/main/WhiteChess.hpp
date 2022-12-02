#ifndef _WHITECHESS_HPP
#define _WHITECHESS_HPP

#include "Chess.hpp"
#include "Config.hpp"

class WhiteChess : public Chess
{
public:
    WhiteChess(int x, int y) : Chess(x, y, WHITE) {;}

    void Show()
    {
        printf("\033[%d;%dH%s", GetY(), GetX() - 1, WHITE_COLOR);
        printf("\033[%d;%dH", GetY(), GetX());
    }
};

#endif // end of WhiteChess.hpp