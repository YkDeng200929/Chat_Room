#ifndef _BLACKCHESS_HPP
#define _BLACKCHESS_HPP

#include "Chess.hpp"
#include "Config.hpp"

class BlackChess : public Chess
{
public:
    BlackChess(int x, int y) : Chess(x, y, BLACK) {;}

    void Show()
    {
        printf("\033[%d;%dH%s", GetY(), GetX() - 1, BLACK_COLOR);
        printf("\033[%d;%dH", GetY(), GetX());
    }
};
 
#endif // end of BlackChess.hpp