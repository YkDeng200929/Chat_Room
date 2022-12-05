#ifndef _CURSOR_HPP
#define _CURSOR_HPP

#include "Config.hpp"
#include <cstdio>

class Cursor
{
public:
    Cursor() : m_x(CENTER_X), m_y(CENTER_Y)
    {
        Show();
    }

    void Show()
    {
        printf("\033[%d;%dH", m_y, m_x);
    }

    void MoveUp()
    {
        if (m_y - LINE_INTERVAL <= 0)
        {
            return ;
        }
        m_y -= LINE_INTERVAL;
        Show();
    }

    void MoveDown()
    {
        if (m_y + LINE_INTERVAL > MAX_Y)
        {
            return ;
        }
        m_y += LINE_INTERVAL;
        Show();
    }

    void MoveLeft()
    {
        if (m_x - COLUMN_INTERVAL <= 0)
        {
            return ;
        }
        m_x -= COLUMN_INTERVAL;
        Show();
    }

    void MoveRight()
    {
        if (m_x + COLUMN_INTERVAL > MAX_X)
        {
            return ;
        }
        m_x += COLUMN_INTERVAL;
        Show();
    }

    int GetX() const {return m_x;}
    int GetY() const {return m_y;}

private:
    int m_x;
    int m_y;
};

#endif