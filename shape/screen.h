//=== Τΰιλ screen.h ===
#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>

const int XMAX = 120;
const int YMAX = 50;

class point {
public:
    int x, y;
    point(int a = 0, int b = 0) : x(a), y(b) {}
};

void put_point(int a, int b);
void put_point(point p) { put_point(p.x, p.y); }
void put_line(int, int, int, int);
void put_line(point a, point b) { put_line(a.x, a.y, b.x, b.y); }
void screen_init();
void screen_destroy();
void screen_refresh();
void screen_clear();

#endif
