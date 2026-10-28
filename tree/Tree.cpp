#include "Tree.h"

Tree::Tree(char start_tag, char end_tag, int max_d, int max_n, int s_rows, int s_cols) :
    root(nullptr), current_tag(start_tag), max_tag(end_tag),
    max_depth(max_d), max_nodes(max_n), node_count(0),
    screen_rows(s_rows), screen_cols(s_cols) {

    // ИСПРАВЛЕНО: добавлена проверка и инициализация экрана
    SCREEN = new char*[screen_rows];
    for(int i = 0; i < screen_rows; ++i) {
        SCREEN[i] = new char[screen_cols];
    }
}

Tree::~Tree() {
    for(int i = 0; i < screen_rows; ++i) {
        delete [] SCREEN[i];
    }
    delete [] SCREEN;
    delete root;
}

void Tree::clearScreen() {
    for(int i = 0; i < screen_rows; i++) {
        memset(SCREEN[i], '.', screen_cols);
        SCREEN[i][screen_cols-1] = '\0';
    }
}

// ИСПРАВЛЕНО: добавлены ограничения глубины и количества узлов
Node* Tree::createNode(int depth) {
    // Проверка всех ограничений
    if (depth >= max_depth || node_count >= max_nodes || current_tag > max_tag) {
        return nullptr;
    }

    // Вероятность создания уменьшается с глубиной
    bool should_create = (rand() % (depth + 2)) > 0;
    if (!should_create) {
        return nullptr;
    }

    Node *node = new Node();
    node_count++; // Счетчик узлов

    // Создаем потомков (обратный порядок)
    node->lft = createNode(depth + 1);
    node->rgt = createNode(depth + 1);

    // ИСПРАВЛЕНО: обратная разметка - после создания потомков
    node->d = current_tag++;

    return node;
}

void Tree::drawNode(Node *v, int row, int col, int offset) {
    // ИСПРАВЛЕНО: проверка границ экрана
    if (!v || row >= screen_rows || col < 0 || col >= screen_cols) return;

    SCREEN[row][col] = v->d;

    if (row + 1 < screen_rows) {
        int new_offset = offset / 2;
        if (v->lft) drawNode(v->lft, row + 1, col - new_offset, new_offset);
        if (v->rgt) drawNode(v->rgt, row + 1, col + new_offset, new_offset);
    }
}

// ИСПРАВЛЕНО: убраны дорогие операции с вектором
int Tree::countLeftLeaves(Node *v) {
    if (!v) return 0;

    int count = 0;
    // Левый лист: есть левый сын и у него нет потомков
    if (v->lft && !v->lft->lft && !v->lft->rgt) {
        count = 1;
    }

    return count + countLeftLeaves(v->lft) + countLeftLeaves(v->rgt);
}

int Tree::calculateHeight(Node *v) {
    if (!v) return 0;
    int left_height = calculateHeight(v->lft);
    int right_height = calculateHeight(v->rgt);
    return 1 + (left_height > right_height ? left_height : right_height);
}

void Tree::generateTree() {
    node_count = 0;
    current_tag = 'a';
    root = createNode(0);
}

int Tree::countLeftLeavesDFS() {
    return countLeftLeaves(root);
}

void Tree::displayTree() {
    clearScreen();
    if (root) {
        drawNode(root, 0, screen_cols / 2, screen_cols / 4);
    }

    for (int i = 0; i < screen_rows; i++) {
        std::cout << SCREEN[i] << std::endl;
    }
}

int Tree::getTreeHeight() {
    return calculateHeight(root);
}
