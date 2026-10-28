//========== Файл shape.cpp ==========
#include <iostream>
#include <locale.h>
#include <list>
#include "screen.h"
#include "shape.h"

using namespace std;

// Реализация функций экрана
char screen[YMAX][XMAX];

void screen_init()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y]) x = white;
}

void screen_destroy()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y]) x = black;
}

bool on_screen(int a, int b)
{
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
    if (on_screen(a, b)) screen[b][a] = black;
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

void screen_clear() { screen_init(); }

void screen_refresh()
{
    for (int y = YMAX - 1; 0 <= y; --y) {
        for (auto x : screen[y])
            std::cout << x;
        std::cout << '\n';
    }
}

// Реализация списка фигур
list<shape*> shape::shapes;

void shape_refresh()
{
    screen_clear();
    for (auto p : shape::shapes) p->draw();
    screen_refresh();
}

// ===== Конструкторы =====
line::line(point a, point b) : w(a), e(b) {}
line::line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {}
rectangle::rectangle(point a, point b) : sw(a), ne(b) {}

// ===== Реализация line =====
point line::north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
point line::south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
point line::east() const { return point(e.x < w.x ? w.x : e.x, (w.y + e.y) / 2); }
point line::west() const { return point(e.x < w.x ? e.x : w.x, (w.y + e.y) / 2); }
point line::neast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? w.y : e.y); }
point line::seast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? e.y : w.y); }
point line::nwest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? w.y : e.y); }
point line::swest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? e.y : w.y); }
void line::move(int a, int b) { w.x += a; w.y += b; e.x += a; e.y += b; }
void line::draw() { put_line(w, e); }
void line::resize(double d) { e.x = w.x + (e.x - w.x) * d; e.y = w.y + (e.y - w.y) * d; }

// ===== Реализация rectangle =====
point rectangle::north() const { return point((sw.x + ne.x) / 2, ne.y); }
point rectangle::south() const { return point((sw.x + ne.x) / 2, sw.y); }
point rectangle::east() const { return point(ne.x, (sw.y + ne.y) / 2); }
point rectangle::west() const { return point(sw.x, (sw.y + ne.y) / 2); }
point rectangle::neast() const { return ne; }
point rectangle::seast() const { return point(ne.x, sw.y); }
point rectangle::nwest() const { return point(sw.x, ne.y); }
point rectangle::swest() const { return sw; }

void rectangle::rotate_right()
{
    int w = ne.x - sw.x, h = ne.y - sw.y;
    sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
}

void rectangle::rotate_left()
{
    int w = ne.x - sw.x, h = ne.y - sw.y;
    ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
}

void rectangle::move(int a, int b)
{
    sw.x += a; sw.y += b; ne.x += a; ne.y += b;
}

void rectangle::resize(double d)
{
    ne.x = sw.x + (ne.x - sw.x) * d;
    ne.y = sw.y + (ne.y - sw.y) * d;
}

void rectangle::draw()
{
    put_line(nwest(), ne);
    put_line(ne, seast());
    put_line(seast(), sw);
    put_line(sw, nwest());
}

void up(shape& p, const shape& q)
{
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

void down(shape& p, const shape& q)
{
    point n = q.south();
    point s = p.north();
    p.move(n.x - s.x, n.y - s.y - 1);
}

// ===== Класс для лица (с глазами, носом, ртом) =====
class myshape : public rectangle {
    int w, h;
    line l_eye;
    line r_eye;
    line mouth;
public:
    myshape(point, point);
    void draw() override;
    void move(int, int) override;
    void resize(double r) override {
        rectangle::resize(r);
        rectangle::move(w * (1 - r) * 0.5, h * (1 - r) * 0.5);
    }
    void rotate_left() override {}
    void rotate_right() override {}
};

myshape::myshape(point a, point b)
    : rectangle(a, b),
    w(neast().x - swest().x + 1),
    h(neast().y - swest().y + 1),
    // Глаза - точки, а не линии! Используем горизонтальные линии длиной 1
    l_eye(point(swest().x + 3, swest().y + h * 3 / 4), 3),  // Точка (линия длиной 1)
    r_eye(point(swest().x + w - 6, swest().y + h * 3 / 4), 3),  // Точка
    mouth(point(swest().x + 3, swest().y + h / 4), w - 6)  // Рот - горизонтальная линия
{}

void myshape::draw()
{
    rectangle::draw();
    int a = (swest().x + neast().x) / 2;
    int b = (swest().y + neast().y) / 2;
    put_line(point(a - 1, b), point(a + 1, b)); // Нос
}

void myshape::move(int a, int b)
{
    rectangle::move(a, b);
    l_eye.move(a, b);
    r_eye.move(a, b);
    mouth.move(a, b);
}

// ===== ТРЕУГОЛЬНИК С КРЕСТОМ (для 16 варианта) =====
struct TriangleVertices {
    point top;
    point left;
    point right;
};

class eq_triangle_with_cross : public rectangle, public reflectable {
public:
    eq_triangle_with_cross(point base_center, int height);
    void draw() override;
    void resize(double d) override;
    point north() const override;
    point south() const override;
    point east() const override;
    point west() const override;
    point neast() const override;
    point seast() const override;
    point nwest() const override;
    point swest() const override;
    void rotate_left() override;
    void rotate_right() override;

    eq_triangle_with_cross(const eq_triangle_with_cross&) = delete;
    eq_triangle_with_cross& operator=(const eq_triangle_with_cross&) = delete;

private:
    int height;
    TriangleVertices get_current_vertices() const;
    point transform_point(point p) const;
};

eq_triangle_with_cross::eq_triangle_with_cross(point base_center, int h)
    : rectangle(
        point(base_center.x - h, base_center.y - h/2),
        point(base_center.x + h, base_center.y + h/2)
      ),
      reflectable(false, false),
      height(h)
{}

point eq_triangle_with_cross::transform_point(point p) const {
    point result = p;
    if (hor) {
        result.x = sw.x + (ne.x - result.x);
    }
    if (vert) {
        result.y = sw.y + (ne.y - result.y);
    }
    return result;
}

TriangleVertices eq_triangle_with_cross::get_current_vertices() const {
    point center((sw.x + ne.x) / 2, (sw.y + ne.y) / 2);
    int half_height = height / 2;
    int half_base = height;

    TriangleVertices verts;

    switch(state) {
        case rotated::no:
            verts.top = point(center.x, ne.y);
            verts.left = point(center.x - half_base, center.y - half_height/2);
            verts.right = point(center.x + half_base, center.y - half_height/2);
            break;
        case rotated::left:
            verts.top = point(sw.x, center.y);
            verts.left = point(center.x - half_height/2, ne.y);
            verts.right = point(center.x - half_height/2, sw.y);
            break;
        case rotated::right:
            verts.top = point(ne.x, center.y);
            verts.left = point(center.x + half_height/2, sw.y);
            verts.right = point(center.x + half_height/2, ne.y);
            break;
    }
    return verts;
}

void eq_triangle_with_cross::draw() {
    TriangleVertices v = get_current_vertices();

    point top = transform_point(v.top);
    point left = transform_point(v.left);
    point right = transform_point(v.right);

    // Рисуем треугольник
    put_line(top, left);
    put_line(left, right);
    put_line(right, top);

    // Рисуем крест внутри
    point center((sw.x + ne.x) / 2, (sw.y + ne.y) / 2);

    point v_start(center.x, sw.y + 2);
    point v_end(center.x, ne.y - 2);
    put_line(transform_point(v_start), transform_point(v_end));

    point h_start(sw.x + 2, center.y);
    point h_end(ne.x - 2, center.y);
    put_line(transform_point(h_start), transform_point(h_end));
}

point eq_triangle_with_cross::north() const {
    TriangleVertices v = get_current_vertices();
    return transform_point(v.top);
}

point eq_triangle_with_cross::south() const {
    TriangleVertices v = get_current_vertices();
    point p1 = transform_point(v.left);
    point p2 = transform_point(v.right);
    return point((p1.x + p2.x)/2, (p1.y < p2.y ? p1.y : p2.y));
}

point eq_triangle_with_cross::east() const {
    TriangleVertices v = get_current_vertices();
    point p1 = transform_point(v.right);
    point p2 = transform_point(v.left);
    point p3 = transform_point(v.top);
    int maxX = p1.x;
    if(p2.x > maxX) maxX = p2.x;
    if(p3.x > maxX) maxX = p3.x;
    return point(maxX, (sw.y + ne.y)/2);
}

point eq_triangle_with_cross::west() const {
    TriangleVertices v = get_current_vertices();
    point p1 = transform_point(v.right);
    point p2 = transform_point(v.left);
    point p3 = transform_point(v.top);
    int minX = p1.x;
    if(p2.x < minX) minX = p2.x;
    if(p3.x < minX) minX = p3.x;
    return point(minX, (sw.y + ne.y)/2);
}

point eq_triangle_with_cross::neast() const { return transform_point(point(ne.x, ne.y)); }
point eq_triangle_with_cross::seast() const { return transform_point(point(ne.x, sw.y)); }
point eq_triangle_with_cross::nwest() const { return transform_point(point(sw.x, ne.y)); }
point eq_triangle_with_cross::swest() const { return transform_point(point(sw.x, sw.y)); }

void eq_triangle_with_cross::resize(double d) {
    rectangle::resize(d);
    height = static_cast<int>(height * d);
}

void eq_triangle_with_cross::rotate_left() {
    rotatable::rotate_left();
}

void eq_triangle_with_cross::rotate_right() {
    rotatable::rotate_right();
}

// ===== ИСПРАВЛЕННЫЕ функции для точного прилегания треугольников =====
void attach_to_hat_bottom(shape& p, const shape& q) {
    // Размещаем треугольник так, чтобы его ВЕРШИНА касалась НИЗА шляпы
    point target = q.south();           // Низ шляпы
    point p_top = p.north();            // Верх треугольника (после отражения)

    // Перемещаем так, чтобы верх треугольника точно совпал с низом шляпы
    p.move(target.x - p_top.x, target.y - p_top.y);
}

void attach_to_brim_left(shape& p, const shape& q) {
    if(auto* rot = dynamic_cast<rotatable*>(&p)) {
        rot->rotate_left();
    }

    // Для левого конца палки треугольник повернут влево
    // Его ПРАВАЯ сторона должна касаться ЛЕВОГО конца палки
    point target = q.west();            // Левый конец палки
    point p_right = p.east();           // Правая сторона треугольника

    // Перемещаем так, чтобы правая сторона касалась левого конца палки
    p.move(target.x - p_right.x - 1, target.y - 3);
}

void attach_to_brim_right(shape& p, const shape& q) {
    if(auto* rot = dynamic_cast<rotatable*>(&p)) {
        rot->rotate_right();
    }

    // Для правого конца палки треугольник повернут вправо
    // Его ЛЕВАЯ сторона должна касаться ПРАВОГО конца палки
    point target = q.east();            // Правый конец палки
    point p_left = p.west();            // Левая сторона треугольника

    // Перемещаем так, чтобы левая сторона касалась правого конца палки
    p.move(target.x - p_left.x + 1, target.y - 3);
}

// ===== ГЛАВНАЯ ФУНКЦИЯ =====
int main()
{
    setlocale(LC_ALL, "Russian");
    screen_init();

    // Базовые фигуры - сдвинуты вправо
    rectangle hat(point(15, 27), point(29, 36));         // Квадрат (ширина 10)
    line brim(point(9, 25), 27);                       // Палка от X=9 до X=35
    myshape face(point(13, 12), point(31, 23));        // Лицо

    // Треугольники с крестом
    eq_triangle_with_cross left_triangle(point(9, 25), 8);   // Левый - большой
    eq_triangle_with_cross right_triangle(point(35, 25), 8); // Правый - большой
    eq_triangle_with_cross bottom_triangle(point(22, 29), 8); // Высота 5, ширина 10

    // Этап 1: Исходный набор
    shape_refresh();
    std::cout << "=== Generated... ===\n";
    std::cin.get();

    // Подготовка
    brim.resize(1.5);             // Удлиняем палку
    face.resize(1.1);              // Немного увеличиваем лицо

    // Подготавливаем треугольники
    left_triangle.rotate_left();     // Левый треугольник поворачиваем влево
    right_triangle.rotate_right();   // Правый треугольник поворачиваем вправо
    bottom_triangle.flip_vertically(); // Нижний треугольник отражаем вниз

    shape_refresh();
    std::cout << "=== Prepared... ===\n";
    std::cin.get();

    // Сборка
    up(brim, face);               // Палка над лицом
    up(hat, brim);                // Шляпа над палкой

    // Размещаем треугольники с точным прилеганием
    attach_to_brim_left(left_triangle, brim);    // Левый треугольник
    attach_to_brim_right(right_triangle, brim);  // Правый треугольник
    attach_to_hat_bottom(bottom_triangle, hat);  // Нижний треугольник

    shape_refresh();
    std::cout << "=== Ready! ===\n";
    std::cin.get();

    screen_destroy();
    return 0;
}
