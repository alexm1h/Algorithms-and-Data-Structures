//======== Τΰιλ shape.h =========
#ifndef SHAPE_H
#define SHAPE_H

#include <list>
#include "screen.h"

using std::list;

extern char screen[YMAX][XMAX];

enum color { black = '*', white = '.' };

bool on_screen(int a, int b);

struct shape {
    static list<shape*> shapes;
    shape() { shapes.push_back(this); }
    virtual point north() const = 0;
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0;
    virtual void move(int, int) = 0;
    virtual void resize(double) = 0;
    virtual ~shape() { shapes.remove(this); }
};

class rotatable : virtual public shape {
protected:
    enum class rotated { left, no, right };
    rotated state;
public:
    rotatable(rotated r = rotated::no) : state(r) {}
    virtual void rotate_left() { state = rotated::left; }
    virtual void rotate_right() { state = rotated::right; }
};

class reflectable : virtual public shape {
protected:
    bool hor, vert;
public:
    reflectable(bool h = false, bool v = false) : hor(h), vert(v) {}
    void flip_horisontally() { hor = !hor; }
    void flip_vertically() { vert = !vert; }
};

class line : public shape {
protected:
    point w, e;
public:
    line(point a, point b);
    line(point a, int L);
    point north() const override;
    point south() const override;
    point east() const override;
    point west() const override;
    point neast() const override;
    point seast() const override;
    point nwest() const override;
    point swest() const override;
    void move(int a, int b) override;
    void draw() override;
    void resize(double d) override;
};

class rectangle : public rotatable {
protected:
    point sw, ne;
public:
    rectangle(point a, point b);
    point north() const override;
    point south() const override;
    point east() const override;
    point west() const override;
    point neast() const override;
    point seast() const override;
    point nwest() const override;
    point swest() const override;
    void rotate_right() override;
    void rotate_left() override;
    void move(int a, int b) override;
    void resize(double d) override;
    void draw() override;
};

void shape_refresh();
void up(shape& p, const shape& q);
void down(shape& p, const shape& q);

#endif
