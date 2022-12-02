#ifndef _CHESSBOARD_HPP
#define _CHESSBOARD_HPP

#include "Chess.hpp"
#include "Config.hpp"

class ChessBoard
{
public:
    static ChessBoard* GetInstance()
    {
        return m_chessBoard;
    }

    void show()
    {
        char buf[1024] = {0};
        FILE *fp = fopen(CHESS_BOARD_FILE, "r");
        if (NULL == fp)
        {
            perror("fopen");
            return ;
        }
        cout << "\033[1;1H" << CHESS_BOARD_COLOR;
        
        while (NULL != fgets(buf, sizeof(buf), fp))
        {
            fputs(buf, stdout);
        }

        cout << "\033[0m";
        fclose(fp);
    } // end of Show

    bool IsValidPosition(int line, int column)
    {
        if (line >= LINE || line < 0 || column >= COLUMN || column < 0)
        {
            return false;
        }
        return NULL == chess[line][column] ? true : false;
    }
    
    bool PlaceChess(Chess *chess)
    {
        int line = (chess->GetY() - 1) / LINE_INTERVAL;
        int column = (chess->GetX() - 1) / COLUMN_INTERVAL;

        if (IsValidPosition(line, column))
        {
            this->chess[line][column] = chess;
            chess->Show();
            return true;
        }
        return false;
    }
// end of ChessBoard public

private:
    ChessBoard()
    {
        for (int i = 0; i < LINE; ++i)
        {
            for (int j = 0; j < COLUMN; ++j)
            {
                chess[i][j] = NULL;
            }
        }
    }
    ~ChessBoard() {;}

    static ChessBoard* m_chessBoard;
    Chess *chess[LINE][COLUMN];
};
ChessBoard* ChessBoard::m_chessBoard = new ChessBoard;

#endif // end of ChessBoard