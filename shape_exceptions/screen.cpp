#include "screen.h"
#include "exceptions.h"

// Внешние переменные из shape.cpp
extern char screen[YMAX][XMAX];
extern bool on_screen(int a, int b);
bool on_screen(int a, int b);

void put_point(int a, int b)
{
    if (on_screen(a, b)) {
        screen[b][a] = '*';
    } else {
        throw OutOfScreenError(a, b, XMAX, YMAX);
    }
}

void put_line(int x0, int y0, int x1, int y1)
{
    int dx = 1;
    int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
    int dy = 1;
    int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
    int two_a = 2 * a;
    int two_b = 2 * b;
    int xcrit = -b + two_a;
    int eps = 0;
    for (;;) {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        if (eps <= xcrit) x0 += dx, eps += two_b;
        if (eps >= a || a < b) y0 += dy, eps -= two_a;
    }
}

void screen_init()
{
    for (int y = 0; y < YMAX; ++y)
        for (int x = 0; x < XMAX; ++x)
            screen[y][x] = '.';
}

void screen_destroy()
{
    // Не обязательная функция
}

void screen_clear()
{
    screen_init();
}

void screen_refresh()
{
    for (int y = YMAX - 1; y >= 0; --y) {
        for (int x = 0; x < XMAX; ++x)
            std::cout << screen[y][x];
        std::cout << '\n';
    }
}
