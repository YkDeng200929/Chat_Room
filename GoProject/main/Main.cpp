#include "WhiteChess.hpp"
#include "BlackChess.hpp"
#include "WhitePlayer.hpp"
#include "BlackPlayer.hpp"
#include "KeyHandle.hpp"
#include "Arbitration.hpp"

int main(void)
{
    cout << "\033[2J";      // 清空屏幕

    ChessBoard *chessBoard = ChessBoard::GetInstance();
    chessBoard->show();

    Player *player[2];
    player[0] = new BlackPlayer("Ykdeng1");
    player[1] = new WhitePlayer("Ykdeng2");

    KeyHandle key;
    Arbitration arb;
    bool isWin = false;

    while (1)
    {
        for (int j = 0; j < 2; ++j)
        {
            key.WaitPlaceChess(player[j]);
            isWin = arb.IsWin(player[j]->GetColor());
            if (isWin)
            {
                cout << "\033[33;0H" << player[j]->GetName() << "Win";
                break;
            }
        }
        if (isWin)
        {
            break;
        }
    }

    cout << "\033[0m";      // 清除格式
    cout << "\033[35;0H";   // 把光标位置设下面点

    return 0;
}