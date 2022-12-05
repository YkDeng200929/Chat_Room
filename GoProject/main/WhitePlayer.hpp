#ifndef _WHITEPLAYER_HPP
#define _WHITEPLAYER_HPP

#include "Player.hpp"
#include "Config.hpp"
#include "WhiteChess.hpp"

class WhitePlayer : public Player
{
public:
    WhitePlayer(const string &name) : Player(name, WHITE) {;}

    bool PlaceChess(int x, int y)
    {
        Chess *whiteChess = new WhiteChess(x, y);
        ChessBoard *chessBoard = ChessBoard::GetInstance();
        
        if (!chessBoard->PlaceChess(whiteChess))
        {
            delete whiteChess;
            return false;
        }
        return true;
    }
};

#endif // end of WhitePlayer.hpp