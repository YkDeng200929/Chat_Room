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
            m_curLine = line;
            m_curColumn = column;
            return true;
        }
        return false;
    }

    bool IsValidColor(int line, int column, char color)
    {
        if (line < 0 || line >= LINE || column < 0 || column >= COLUMN)
        {
            return false;
        }
        Chess *ch = chess[line][column];
        if (NULL == ch)
        {
            return false;
        }
        if (ch->GetColor() != color)
        {
            return false;
        }
        return true;
    }

    int GetCurLine() const {return m_curLine;}
    int GetCurColumn() const {return m_curColumn;}

// end of ChessBoard public

private:
    ChessBoard() : m_curLine(-1), m_curColumn(-1)
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
    int m_curLine;
    int m_curColumn;
};
ChessBoard* ChessBoard::m_chessBoard = new ChessBoard;

#endif // end of ChessBoard