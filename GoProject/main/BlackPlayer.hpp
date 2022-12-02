#ifndef _BLACKPLAYER_HPP
#define _BLACKPLAYER_HPP

#include "Player.hpp"
#include "Config.hpp"
#include "BlackChess.hpp"

class BlackPlayer : public Player
{
public:
    BlackPlayer(const string &name) : Player(name, BLACK) {;}

    bool PlaceChess(int x, int y)
    {
        Chess *blackChess = new BlackChess(x, y);
        ChessBoard *chessBoard = ChessBoard::GetInstance();
        
        if (!chessBoard->PlaceChess(blackChess))
        {
            delete blackChess;
            return false;
        }
        return true;
    }
};

#endif // end of BlackPlayer.hpp