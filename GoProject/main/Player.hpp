#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include "ChessBoard.hpp"

class Player
{
public:
    Player(const string& name, char color) : m_name(name),
        m_color(color) {;}
    virtual ~Player() {;}

    virtual bool PlaceChess(int x, int y) = 0;
    string GetName() const {return m_name;}
    char GetColor() const {return m_color;}

private:
    string m_name;
    char m_color;
};

#endif