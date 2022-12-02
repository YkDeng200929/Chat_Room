#include <iostream>
#include <cstdio>
#define EDGE 5

using namespace std;

class Mi
{
public:
    Mi(int x, int y) : m_x(x), m_y(y) {;}

    void DrawPoint(int x, int y, int fgColor, int bgColor)
    {
        printf("\033[%d;%dH\033[%d;%dm*", x, y, fgColor, bgColor);
    }

    void Show(void)
    {
        DrawPoint(m_x, m_y, 31, 42);
        for (int i = 1; i <= EDGE; ++i)
        {
            DrawPoint(m_x - i, m_y - i, 32, 43);
            DrawPoint(m_x + i, m_y + i, 32, 43);

            DrawPoint(m_x, m_y - i, 33, 44);
            DrawPoint(m_x, m_y + i, 33, 44);

            DrawPoint(m_x - i, m_y, 34, 45);
            DrawPoint(m_x + i, m_y, 34, 45);
            
            DrawPoint(m_x + i, m_y - i, 35, 46);
            DrawPoint(m_x - i, m_y + i, 35, 46);
        }
    }
private:
    int m_x;
    int m_y;
};

int main(void)
{
    cout << "\033[2J";

    Mi centerPoint(20, 10);
    centerPoint.Show();

    cout << "\033[0m";
    cout << "\033[30;0H";

    return 0;
}