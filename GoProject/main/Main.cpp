#include "WhiteChess.hpp"
#include "BlackChess.hpp"
#include "WhitePlayer.hpp"
#include "BlackPlayer.hpp"

int main(void)
{
    cout << "\033[2J";      // 清空屏幕

    ChessBoard *chessBoard = ChessBoard::GetInstance();
    chessBoard->show();

    WhitePlayer player("ykdeng");
    player.PlaceChess(17, 5);

    BlackPlayer player2("ykdeng2");
    player2.PlaceChess(9, 5);

    cout << "\033[0m";      // 清除格式
    cout << "\033[35;0H";   // 把光标位置设下面点

    return 0;
}