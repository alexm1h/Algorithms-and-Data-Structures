#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <memory>
#include <queue>
#include <sstream>
#include <iomanip>
#include <Windows.h>
using namespace std;

// Узел дерева с хранением высоты
template<typename T>
struct TreeNode {
    T key;
    int height;
    TreeNode* left;
    TreeNode* right;
    TreeNode* next; // для последовательности

    TreeNode(const T& k) : key(k), height(1), left(nullptr), right(nullptr), next(nullptr) {}
};

// ДДПв с автобалансировкой (AVL-подобное)
template<typename T>
class AVLTree {
private:
    TreeNode<T>* root;
    TreeNode<T>* sequenceHead; // голова последовательности
    TreeNode<T>* sequenceTail; // хвост последовательности
    size_t treeSize;

    int getHeight(TreeNode<T>* node) {
        return node ? node->height : 0;
    }

    void updateHeight(TreeNode<T>* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    int getBalance(TreeNode<T>* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    TreeNode<T>* rotateRight(TreeNode<T>* y) {
        TreeNode<T>* x = y->left;
        TreeNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    TreeNode<T>* rotateLeft(TreeNode<T>* x) {
        TreeNode<T>* y = x->right;
        TreeNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    TreeNode<T>* balance(TreeNode<T>* node) {
        if (!node) return nullptr;

        updateHeight(node);
        int balance = getBalance(node);

        // Левое поддерево тяжелее
        if (balance > 1) {
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        // Правое поддерево тяжелее
        if (balance < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    TreeNode<T>* insertNode(TreeNode<T>* node, const T& key) {
        if (!node) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            // Добавление в последовательность
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
            treeSize++;
            return newNode;
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key);
        } else {
            return node; // дубликаты не вставляем для множества
        }

        return balance(node);
    }

    TreeNode<T>* findMin(TreeNode<T>* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    TreeNode<T>* removeNode(TreeNode<T>* node, const T& key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeNode(node->left, key);
        } else if (key > node->key) {
            node->right = removeNode(node->right, key);
        } else {
            // Нашли узел для удаления
            if (!node->left || !node->right) {
                TreeNode<T>* temp = node->left ? node->left : node->right;
                delete node;
                treeSize--;
                return temp;
            } else {
                TreeNode<T>* temp = findMin(node->right);
                node->key = temp->key;
                node->right = removeNode(node->right, temp->key);
            }
        }

        return balance(node);
    }

    bool containsNode(TreeNode<T>* node, const T& key) const {
        if (!node) return false;
        if (key < node->key) return containsNode(node->left, key);
        if (key > node->key) return containsNode(node->right, key);
        return true;
    }

    void inorderCollect(TreeNode<T>* node, vector<T>& result) const {
        if (!node) return;
        inorderCollect(node->left, result);
        result.push_back(node->key);
        inorderCollect(node->right, result);
    }

    void destroyTree(TreeNode<T>* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr), sequenceHead(nullptr), sequenceTail(nullptr), treeSize(0) {}

    ~AVLTree() {
        destroyTree(root);
    }

    void insert(const T& key) {
        if (!contains(key)) {
            root = insertNode(root, key);
        }
    }

    bool remove(const T& key) {
        if (contains(key)) {
            // Удаление из последовательности
            TreeNode<T>* prev = nullptr;
            TreeNode<T>* curr = sequenceHead;
            while (curr && curr->key != key) {
                prev = curr;
                curr = curr->next;
            }
            if (curr) {
                if (prev) prev->next = curr->next;
                else sequenceHead = curr->next;
                if (curr == sequenceTail) sequenceTail = prev;
            }

            root = removeNode(root, key);
            return true;
        }
        return false;
    }

    bool contains(const T& key) const {
        return containsNode(root, key);
    }

    size_t size() const {
        return treeSize;
    }

    bool empty() const {
        return treeSize == 0;
    }

    vector<T> toVector() const {
        vector<T> result;
        inorderCollect(root, result);
        return result;
    }

    // Получение последовательности (порядок вставки)
    vector<T> getSequence() const {
        vector<T> result;
        TreeNode<T>* curr = sequenceHead;
        while (curr) {
            result.push_back(curr->key);
            curr = curr->next;
        }
        return result;
    }

    // Установка последовательности из вектора (для операций)
    void setSequence(const vector<T>& seq) {
        sequenceHead = sequenceTail = nullptr;
        for (const auto& key : seq) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
        }
    }

    // Операция ERASE - удаление части последовательности между позициями p1 и p2
    void eraseSequence(size_t p1, size_t p2) {
        vector<T> seq = getSequence();
        if (p1 >= seq.size() || p2 >= seq.size() || p1 > p2) {
            throw out_of_range("Invalid positions for ERASE");
        }

        vector<T> newSeq;
        for (size_t i = 0; i < seq.size(); i++) {
            if (i < p1 || i > p2) {
                newSeq.push_back(seq[i]);
            } else {
                // Удаляем элементы из множества
                remove(seq[i]);
            }
        }

        // Восстанавливаем последовательность из оставшихся элементов
        // и добавляем обратно в дерево элементы, которые были удалены из последовательности,
        // но могут быть в других множествах? Нет, для этой операции удаляем полностью
        sequenceHead = sequenceTail = nullptr;
        for (const auto& key : newSeq) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
        }
    }

    // Операция SUBST - вставка последовательности B в позицию p
    void substSequence(const vector<T>& seqB, size_t p) {
        vector<T> seqA = getSequence();
        if (p > seqA.size()) {
            throw out_of_range("Invalid position for SUBST");
        }

        vector<T> newSeq;
        for (size_t i = 0; i < p; i++) {
            newSeq.push_back(seqA[i]);
        }
        for (const auto& key : seqB) {
            newSeq.push_back(key);
            insert(key); // добавляем в множество, если еще нет
        }
        for (size_t i = p; i < seqA.size(); i++) {
            newSeq.push_back(seqA[i]);
        }

        setSequence(newSeq);
    }

    // Операция CHANGE - замена элементов, начиная с позиции p, последовательностью B
    void changeSequence(const vector<T>& seqB, size_t p) {
        vector<T> seqA = getSequence();
        if (p > seqA.size()) {
            throw out_of_range("Invalid position for CHANGE");
        }

        // Удаляем заменяемые элементы из множества
        for (size_t i = p; i < seqA.size() && i < p + seqB.size(); i++) {
            remove(seqA[i]);
        }

        vector<T> newSeq;
        for (size_t i = 0; i < p; i++) {
            newSeq.push_back(seqA[i]);
        }
        for (const auto& key : seqB) {
            newSeq.push_back(key);
            insert(key);
        }
        for (size_t i = p + seqB.size(); i < seqA.size(); i++) {
            newSeq.push_back(seqA[i]);
        }

        setSequence(newSeq);
    }

    void print() const {
        vector<T> vec = toVector();
        cout << "Множество (упорядоченное): { ";
        for (const auto& key : vec) {
            cout << key << " ";
        }
        cout << "}" << endl;

        vector<T> seq = getSequence();
        cout << "Последовательность (порядок вставки): < ";
        for (const auto& key : seq) {
            cout << key << " ";
        }
        cout << ">" << endl;
    }
};

// Операции над множествами
template<typename T>
AVLTree<T> symmetricDifference(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();
    vector<T> vecB = B.toVector();

    // Элементы, которые есть в A, но нет в B
    for (const auto& key : vecA) {
        if (!B.contains(key)) {
            result.insert(key);
        }
    }

    // Элементы, которые есть в B, но нет в A
    for (const auto& key : vecB) {
        if (!A.contains(key)) {
            result.insert(key);
        }
    }

    return result;
}

template<typename T>
AVLTree<T> intersection(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();

    for (const auto& key : vecA) {
        if (B.contains(key)) {
            result.insert(key);
        }
    }

    return result;
}

template<typename T>
AVLTree<T> difference(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();

    for (const auto& key : vecA) {
        if (!B.contains(key)) {
            result.insert(key);
        }
    }

    return result;
}

// Генерация случайных множеств
AVLTree<int> generateRandomSet(size_t size, int minVal = 0, int maxVal = 200) {
    AVLTree<int> result;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(minVal, maxVal);

    while (result.size() < size) {
        result.insert(dis(gen));
    }

    return result;
}

// Создание последовательности для операций
vector<int> generateRandomSequence(size_t size, int minVal = 0, int maxVal = 200) {
    vector<int> result;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(minVal, maxVal);

    for (size_t i = 0; i < size; i++) {
        result.push_back(dis(gen));
    }

    return result;
}

// Вычисление выражения ((A ⊕ (B ∩ C)) \ D) ∩ E
AVLTree<int> evaluateExpression(const AVLTree<int>& A, const AVLTree<int>& B,
                                 const AVLTree<int>& C, const AVLTree<int>& D,
                                 const AVLTree<int>& E) {
    // Шаг 1: B ∩ C
    cout << "\n1. Вычисляем B ∩ C:" << endl;
    AVLTree<int> step1 = intersection(B, C);
    step1.print();

    // Шаг 2: A ⊕ (B ∩ C)
    cout << "\n2. Вычисляем A ⊕ (B ∩ C):" << endl;
    AVLTree<int> step2 = symmetricDifference(A, step1);
    step2.print();

    // Шаг 3: (A ⊕ (B ∩ C)) \ D
    cout << "\n3. Вычисляем (A ⊕ (B ∩ C)) \\ D:" << endl;
    AVLTree<int> step3 = difference(step2, D);
    step3.print();

    // Шаг 4: ((A ⊕ (B ∩ C)) \ D) ∩ E
    cout << "\n4. Вычисляем ((A ⊕ (B ∩ C)) \\ D) ∩ E:" << endl;
    AVLTree<int> result = intersection(step3, E);
    result.print();

    return result;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    try {
        cout << "========================================" << endl;
        cout << "Лабораторная работа №3" << endl;
        cout << "Вариант 16" << endl;
        cout << "Выражение: (A ⊕ B ∩ C) \\ D ∩ E" << endl;
        cout << "Структура: ДДПв (AVL-дерево с высотой)" << endl;
        cout << "Операции с последовательностью: ERASE, SUBST, CHANGE" << endl;
        cout << "========================================" << endl;

        // Генерация исходных множеств мощностью 32
        cout << "\nГенерация исходных множеств (мощность 32):" << endl;
        cout << "----------------------------------------" << endl;

        AVLTree<int> A = generateRandomSet(32, 0, 100);
        AVLTree<int> B = generateRandomSet(32, 50, 150);
        AVLTree<int> C = generateRandomSet(32, 100, 200);
        AVLTree<int> D = generateRandomSet(32, 0, 100);
        AVLTree<int> E = generateRandomSet(32, 50, 150);

        cout << "Множество A: ";
        A.print();
        cout << "Множество B: ";
        B.print();
        cout << "Множество C: ";
        C.print();
        cout << "Множество D: ";
        D.print();
        cout << "Множество E: ";
        E.print();

        // Вычисление выражения
        cout << "\n========================================" << endl;
        cout << "Вычисление выражения (A ⊕ B ∩ C) \\ D ∩ E" << endl;
        cout << "========================================" << endl;

        AVLTree<int> result = evaluateExpression(A, B, C, D, E);

        cout << "\n========================================" << endl;
        cout << "Результат вычисления выражения:" << endl;
        cout << "========================================" << endl;
        result.print();

        // Демонстрация операций с последовательностями
        cout << "\n========================================" << endl;
        cout << "Демонстрация операций с последовательностью" << endl;
        cout << "========================================" << endl;

        // Создаем тестовую последовательность для операций
        vector<int> testSeq = generateRandomSequence(10, 0, 100);
        AVLTree<int> testTree;
        cout << "\nИсходная тестовая последовательность: < ";
        for (const auto& key : testSeq) {
            testTree.insert(key);
            cout << key << " ";
        }
        cout << ">" << endl;
        testTree.print();

        // 1. Операция ERASE
        cout << "\n--- Операция ERASE (удаление элементов с 2 по 5 позицию) ---" << endl;
        try {
            testTree.eraseSequence(2, 5);
            cout << "Результат ERASE(2,5):" << endl;
            testTree.print();
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

        // 2. Операция SUBST
        cout << "\n--- Операция SUBST (вставка последовательности в позицию 3) ---" << endl;
        vector<int> substSeq = {999, 888, 777};
        cout << "Вставляемая последовательность: < ";
        for (const auto& key : substSeq) {
            cout << key << " ";
        }
        cout << ">" << endl;
        try {
            testTree.substSequence(substSeq, 3);
            cout << "Результат SUBST(последовательность, 3):" << endl;
            testTree.print();
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

        // 3. Операция CHANGE
        cout << "\n--- Операция CHANGE (замена с позиции 2) ---" << endl;
        vector<int> changeSeq = {111, 222, 333, 444};
        cout << "Заменяющая последовательность: < ";
        for (const auto& key : changeSeq) {
            cout << key << " ";
        }
        cout << ">" << endl;
        try {
            testTree.changeSequence(changeSeq, 2);
            cout << "Результат CHANGE(последовательность, 2):" << endl;
            testTree.print();
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

        // Анализ временной сложности
        cout << "\n========================================" << endl;
        cout << "Оценка временной сложности" << endl;
        cout << "========================================" << endl;
        cout << "Операции над множествами:" << endl;
        cout << "  - Пересечение (∩): O(n log n)" << endl;
        cout << "  - Симметрическая разность (⊕): O(n log n)" << endl;
        cout << "  - Разность (\\\\): O(n log n)" << endl;
        cout << "  - Вставка/удаление: O(log n)" << endl;
        cout << "  - Поиск: O(log n)" << endl;
        cout << "  - Балансировка: O(log n) после каждой вставки/удаления" << endl;
        cout << endl;
        cout << "Операции с последовательностью:" << endl;
        cout << "  - ERASE: O(n) - просмотр всей последовательности" << endl;
        cout << "  - SUBST: O(n + m) - где m - размер вставляемой последовательности" << endl;
        cout << "  - CHANGE: O(n + m) - где m - размер заменяющей последовательности" << endl;
        cout << endl;
        cout << "Общая сложность выражения (A ⊕ B ∩ C) \\ D ∩ E:" << endl;
        cout << "  - Лучший случай: O(n log n)" << endl;
        cout << "  - Средний случай: O(n log n)" << endl;
        cout << "  - Худший случай: O(n log n) (благодаря балансировке)" << endl;
        cout << "  - Где n = 32 (мощность множеств)" << endl;

        cout << "\n========================================" << endl;
        cout << "Программа успешно завершена!" << endl;
        cout << "========================================" << endl;

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
