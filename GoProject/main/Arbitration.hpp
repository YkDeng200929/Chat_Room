#ifndef _ARBITRATION_HPP
#define _ARBITRATION_HPP

#include "ChessBoard.hpp"

class Arbitration
{
public:
    bool IsWin(char color)
    {
        bool ret = false;
        ret = IsDirectionWin(color, 1, 0);  // 水平方向
        if (ret)
        {
            return true;
        }
        ret = IsDirectionWin(color, 0, 1);  // 垂直方向
        if (ret)
        {
            return true;
        }
        ret = IsDirectionWin(color, 1, 1);  // 下坡方向
        if (ret)
        {
            return true;
        }
        ret = IsDirectionWin(color, 1, -1);  // 上坡方向
        if (ret)
        {
            return true;
        }
        return false;
    }

private:
    bool IsDirectionWin(char color, int hor, int ver)
    {
        ChessBoard *chessBoard = ChessBoard::GetInstance();
        int line = chessBoard->GetCurLine();
        int column = chessBoard->GetCurColumn();
        int count = 1;

        for (int i = 1; i < 5; ++i)
        {
            bool ret = chessBoard->IsValidColor(line + (i * ver),
                                            column + (i * hor), color);
            if (!ret)
            {
                break;
            }
            ++count;
        }

        for (int i = 1; i < 5; ++i)
        {
            bool ret = chessBoard->IsValidColor(line - (i * ver),
                                            column - (i * hor), color);
            if (!ret)
            {
                break;
            }
            ++count;
        }
        cout << "\033[32;0H" << count << endl;
        return count >= 5 ? true : false;
    }
};

#endif