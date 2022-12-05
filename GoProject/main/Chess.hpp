#ifndef _CHESS_HPP
#define _CHESS_HPP

#include <iostream>
#include <cstdio>

using namespace std;

class Chess
{
public:
    Chess(int x, int y, char color) : m_x(x), 
        m_y(y), 
        m_color(color) {;}

    virtual ~Chess() {;}

    int GetX() const {return m_x;}
    int GetY() const {return m_y;}
    char GetColor() const {return m_color;}

    virtual void Show() = 0;
private:
    int m_x;
    int m_y;
    char m_color;
};

#endif