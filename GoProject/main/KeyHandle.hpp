#ifndef _KEYHANDLE_HPP
#define _KEYHANDLE_HPP

#include "Cursor.hpp"
#include "Player.hpp"
#include <cstdlib>

class KeyHandle
{
public:
    KeyHandle()
    {
        system("stty -icanon");
        system("stty -echo");
    }
    ~KeyHandle()
    {
        system("stty icanon");
        system("stty echo");
    }

    void WaitPlaceChess(Player *player)
    {
        char key = 0;
        bool ret = false;

        while (1)
        {
            key = getchar();

            switch (key)
            {
                case 'w':
                case 'W':
                    cursor.MoveUp();
                    break;
                case 's':
                case 'S':
                    cursor.MoveDown();
                    break;
                case 'a':
                case 'A':
                    cursor.MoveLeft();
                    break;
                case 'd':
                case 'D':
                    cursor.MoveRight();
                    break;
                case ' ':
                    ret = player->PlaceChess(cursor.GetX(), cursor.GetY());
                    break;
            }
            if (ret)
            {
                break;
            }
        }
    }
private:
    Cursor cursor;
};

#endif // !_KEYHANDLE_HPP
